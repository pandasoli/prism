#include <stdio.h>
#include <assert.h>
#include <lexer.h>
#include <node.h>

char *token_strkind(kind) TokenKind kind; {
	switch (kind) {
#define X(kind)    case kind ## _TK: return #kind;
#define K(kind, _) case kind ## _KW: return #kind;
	TOKENKINDS
#undef X
#undef K
		default: assert(0);
	}
}

print_node(src, node) char *src; Node *node; {
	if (node == NULL) return 0;

	int err;

	switch (node->kind) {
		case LIT_NK: {
			Lexer l = { src, node->lit.start };
			Token tok;

			if ((err = lex(&l, &tok))) return err;

			unsigned long len = l.pos - node->lit.start;

			printf("%.*s", (int) len, src + node->lit.start);
		} break;

		case BINARY_NK:
			putchar('(');
			print_node(src, node->binary.left);

			printf(" %s ", token_strkind(node->binary.op));

			print_node(src, node->binary.right);
			putchar(')');
			break;

		case UNARY_NK:
			printf("(%s ", token_strkind(node->unary.op));
			print_node(src, node->unary.val);
			putchar(')');
			break;

		case IF_NK:
			printf("if ");
			print_node(src, node->if_.cmp);
			printf(" { ");
			print_node(src, node->if_.body);
			printf(" } ");
			break;

		case ELSE_NK:
			printf("else {");
			print_node(src, node->else_.body);
			printf(" } ");
			break;

		default: assert(0);
	}

	if (node->next) {
		printf(", ");
		print_node(src, node->next);
	}

	return 0;
}
