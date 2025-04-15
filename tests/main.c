#include <lexer.h>
#include <parser.h>
#include <debug.h>


int main(void) {
	/* (((12 + ((4 / 5) * asd)) -1) and (a != 4)) */
	char src[] = "if a + 1 == 6 or a == 6 {} else if {} {}";
	Lexer l = { src, 0 };
	Parser p = { (Token) {}, &l };
	Node *n;
	int err;

	err = parse(&p, &n);
	if (err) return err;

	print_node(src, n);

/*	do {*/
/*		err = lex(&l, &tok);*/
/*		if (err) break;*/
/**/
/*		int len = l.pos - tok.start;*/
/*		char *kind_img;*/
/**/
/*		switch (tok.kind) {*/
/*#define X(kind) case kind ## _TK: kind_img = #kind; break;*/
/*#define K(kind, img) case kind ## _KW: kind_img = #kind; break;*/
/*			TOKENKINDS*/
/*#undef X*/
/*#undef K*/
/*		}*/
/**/
/*		printf("%s %.*s\n", kind_img, len, l.src + tok.start);*/
/*	}*/
/*	while (tok.kind != EOI_TK);*/

	return 0;
}
