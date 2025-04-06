#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lexer.h>
#include <parser.h>
#include <debug.h>

#define CURRENT self->current
#define NEXT lex(self->lexer, &CURRENT)

/* A short for the toppest expr */
#define expr logic
#define ERR(e) if ((err = e)) return err

static logic(Parser *, Node **);

static factor(self, node) Parser *self; Node **node; {
	int err;

	switch (CURRENT.kind) {
		case FLOAT_TK:
		case INT_TK:
		case IDENT_TK:
		case STRING_TK:
		case CHAR_TK: {
			Node *n = malloc(sizeof(Node));
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
			ERR(expr(self, node));

			if (CURRENT.kind != CLOSE_PAREN_TK) {
				fprintf(stderr, "(%s) at %ld: Expected closing parenthesis\n", __FUNCTION__, CURRENT.start);
				return 1;
			}

			ERR(NEXT);
		} break;

		default:
			fprintf(stderr, "(%s) at %ld: Unexpected %s token\n", __FUNCTION__, CURRENT.start, token_strkind(CURRENT.kind));
			return 1;
	}

	return 0;
}

static unary(self, node) Parser *self; Node **node; {
	int err;

	if (CURRENT.kind == SUB_TK) {
		Node *n = malloc(sizeof(Node));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = UNARY_NK;
		n->unary = (UnaryNode) { .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(factor(self, &n->unary.val));

		*node = n;
	}
	else
		return factor(self, node);

	return 0;
}

static mul(self, node) Parser *self; Node **node; {
	int err;
	ERR(unary(self, node));

	while (CURRENT.kind == MUL_TK || CURRENT.kind == DIV_TK) {
		Node *n = malloc(sizeof(Node));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = BINARY_NK;
		n->binary = (BinaryNode) { .left = *node, .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(unary(self, &n->binary.right));

		*node = n;
	}

	return 0;
}

static add(self, node) Parser *self; Node **node; {
	int err;
	ERR(mul(self, node));

	while (CURRENT.kind == ADD_TK || CURRENT.kind == SUB_TK) {
		Node *n = malloc(sizeof(Node));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = BINARY_NK;
		n->binary = (BinaryNode) { .left = *node, .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(mul(self, &n->binary.right));

		*node = n;
	}

	return 0;
}

static cmp(self, node) Parser *self; Node **node; {
	int err;
	ERR(add(self, node));

	while (
		CURRENT.kind == EQUAL_TK || CURRENT.kind == NOT_EQUAL_TK ||
		CURRENT.kind == LESS_TK || CURRENT.kind == LESS_EQUAL_TK ||
		CURRENT.kind == GREATER_TK || CURRENT.kind == GREATER_EQUAL_TK
	) {
		Node *n = malloc(sizeof(Node));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = BINARY_NK;
		n->binary = (BinaryNode) { .left = *node, .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(add(self, &n->binary.right));

		*node = n;
	}

	return 0;
}

static logic(self, node) Parser *self; Node **node; {
	int err;
	ERR(cmp(self, node));

	while (CURRENT.kind == LOGIC_AND_KW || CURRENT.kind == LOGIC_OR_KW) {
		Node *n = malloc(sizeof(Node));
		if (n == NULL) {
			fprintf(stderr, "(%s) at %ld: No more memory\n", __FUNCTION__, CURRENT.start);
			return 1;
		}

		n->kind = BINARY_NK;
		n->binary = (BinaryNode) { .left = *node, .op = CURRENT.kind };
		n->next = NULL;

		ERR(NEXT);
		ERR(cmp(self, &n->binary.right));

		*node = n;
	}

	return 0;
}

parse(self, node) Parser *self; Node **node; {
	assert(self != NULL);
	assert(node != NULL);

	int err;
	ERR(NEXT);

	Node *new, *tail = NULL;
	*node = NULL;

	while (CURRENT.kind != EOI_TK) {
		ERR(logic(self, &new));

		/* Linked list append */
		tail = *node == NULL
			? (*node = new)
			: (tail->next = new);
	}

	return 0;
}
