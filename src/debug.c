#include <stdio.h>
#include <assert.h>
#include <lexer.h>
#include <node.h>

char *prism_token_strkind(kind) PrismTokenKind kind; {
	switch (kind) {
#define X(kind)    case kind ## _TK: return #kind;
#define K(kind, _) case kind ## _KW: return #kind;
	TOKENKINDS
#undef X
#undef K
		default: assert(0);
	}
}

prism_print_node(src, node) char *src; PrismNode *node; {
	if (node == NULL) return 0;

	int err;

	switch (node->kind) {
		case LIT_NK: {
			PrismLexer l = { src, node->lit.start };
			PrismToken tok;

			if ((err = prism_lex(&l, &tok))) return err;

			unsigned long len = l.pos - node->lit.start;

			printf("%.*s", (int) len, src + node->lit.start);
		} break;

		case BINARY_NK:
			putchar('(');
			prism_print_node(src, node->binary.left);

			printf(" %s ", prism_token_strkind(node->binary.op));

			prism_print_node(src, node->binary.right);
			putchar(')');
			break;

		case UNARY_NK:
			printf("(%s ", prism_token_strkind(node->unary.op));
			prism_print_node(src, node->unary.val);
			putchar(')');
			break;

		case IF_NK:
			printf("if ");
			prism_print_node(src, node->if_.cmp);
			printf(" { ");
			prism_print_node(src, node->if_.body);
			printf(" } ");
			break;

		case ELSE_NK:
			printf("else {");
			prism_print_node(src, node->else_.body);
			printf(" } ");
			break;

		default: assert(0);
	}

	if (node->next) {
		printf(", ");
		prism_print_node(src, node->next);
	}

	return 0;
}

