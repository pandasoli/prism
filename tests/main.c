#include <lexer.h>
#include <parser.h>
#include <debug.h>


int main(void) {
	/* (((12 + ((4 / 5) * asd)) -1) and (a != 4)) */
	char src[] = "if a + 1 == 6 or a == 6 {} else if {} {}";
	PrismLexer l = { src, 0 };
	PrismParser p = { (PrismToken) {}, &l };
	PrismNode *n;
	int err;

	err = prism_parse(&p, &n);
	if (err) return err;

	prism_print_node(src, n);

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
