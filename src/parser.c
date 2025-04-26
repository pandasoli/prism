#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lexer.h>
#include <parser.h>
#include <debug.h>

#define CURRENT self->current
#define NEXT prism_lex(self->lexer, &CURRENT)

/* A short for the toppest expr */
#define expr binary
#define ERR(e) if ((err = e)) return err

static binary(PrismParser *, PrismNode **, int);

static factor(self, node) PrismParser *self; PrismNode **node; {
	int err;

	switch (CURRENT.kind) {
		case FLOAT_TK:
		case INT_TK:
		case IDENT_TK:
		case STRING_TK:
		case CHAR_TK: {
			PrismNode *n = malloc(sizeof(PrismNode));
			if (n == NULL) {
				fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
				return 1;
			}

			n->kind = LIT_NK;
			n->lit = CURRENT;
			n->next = NULL;
			*node = n;

			ERR(NEXT);
		} break;

		case OPEN_PAREN_TK: {
			ERR(NEXT);
			ERR(expr(self, node, 0));

			if (CURRENT.kind != CLOSE_PAREN_TK) {
				fprintf(stderr, "(%s) at %ld: Expected closing parenthesis\n", __FUNCTION__, CURRENT.start);
				return 1;
			}

			ERR(NEXT);
		} break;

		case OPEN_BRACE_TK: {
			/*ERR(NEXT);*/
			ERR(prism_parse(self, node));

			if (CURRENT.kind != CLOSE_BRACE_TK) {
				fprintf(stderr, "(%s) at %ld: Expected closing brace\n", __FUNCTION__, CURRENT.start);
				return 1;
			}

			ERR(NEXT);
		} break;

		default:
			fprintf(stderr, "(%s) at %ld: Unexpected %s token\n", __FUNCTION__, CURRENT.start, prism_token_strkind(CURRENT.kind));
			return 1;
	}

	return 0;
}

static unary(self, node) PrismParser *self; PrismNode **node; {
	int err;

	if (CURRENT.kind == SUB_TK) {
		PrismNode *n = malloc(sizeof(PrismNode));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = UNARY_NK;
		n->unary = (PrismUnaryNode) { .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(factor(self, &n->unary.val));

		*node = n;
	}
	else
		return factor(self, node);

	return 0;
}

static get_binprecedence(kind) PrismTokenKind kind; {
	switch (kind) {
		case MUL_TK: case DIV_TK:
			return 4;
		case ADD_TK: case SUB_TK:
			return 3;
		case EQUAL_TK: case NOT_EQUAL_TK: case LESS_TK: case LESS_EQUAL_TK: case GREATER_TK: case GREATER_EQUAL_TK:
			return 2;
		case LOGIC_AND_KW: case LOGIC_OR_KW:
			return 1;
		default:
			return 0;
	}
}

static binary(self, node, parent_precedence) PrismParser *self; PrismNode **node; int parent_precedence; {
	int err;
	ERR(unary(self, node));

	for (;;) {
		int precedence = get_binprecedence(CURRENT.kind);
		if (precedence == 0 || precedence <= parent_precedence)
			break;

		printf("%d\n", precedence);

		PrismNode *n = malloc(sizeof(PrismNode));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = BINARY_NK;
		n->binary = (PrismBinaryNode) { .left = *node, .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(binary(self, &n->binary.right, precedence));

		*node = n;
	}

	return 0;
}

static stmt(self, node) PrismParser *self; PrismNode **node; {
	int err;

	if (CURRENT.kind == IF_KW) {
		ERR(NEXT);

		PrismNode *cmp;
		ERR(binary(self, &cmp, 0));

		PrismNode *body;
		ERR(stmt(self, &body));

		PrismNode *n = malloc(sizeof(PrismNode));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = IF_NK;
		n->if_ = (PrismIfNode) { cmp, body };
		n->next = NULL;

		*node = n;
	}
	else if (CURRENT.kind == ELSE_KW) {
		if ((*node)->kind != IF_NK) {
			fprintf(stderr, "(%s) at %ld: Expected 'if' before 'else'\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		ERR(NEXT);

		PrismNode *body;
		ERR(stmt(self, &body));

		PrismNode *n = malloc(sizeof(PrismNode));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = ELSE_NK;
		n->else_ = (PrismElseNode) { body };
		n->next = NULL;

		*node = n;
	}
	else
		return binary(self, node, 0);

	return 0;
}

prism_parse(self, node) PrismParser *self; PrismNode **node; {
	assert(self != NULL);
	assert(node != NULL);

	int err;
	ERR(NEXT);

	PrismNode *new, *tail = NULL;
	*node = NULL;

	while (CURRENT.kind != EOI_TK && CURRENT.kind != CLOSE_BRACE_TK) {
		ERR(stmt(self, &new));

		/* Linked list append */
		tail = *node == NULL
			? (*node = new)
			: (tail->next = new);
	}

	return 0;
}
