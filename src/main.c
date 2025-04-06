#include <stdio.h>
#include <lexer.h>
#include <token.h>

main() {
	Lexer l = { "12 +\r4 /\n5 * asd\t-1 and a != 4", 0 };
	Token tok;
	int err;

	do {
		err = lex(&l, &tok);
		if (err) break;

		int len = l.pos - tok.start;
		char *kind_img;

		switch (tok.kind) {
#define X(kind) case kind ## _TK: kind_img = #kind; break;
#define K(kind, img) case kind ## _KW: kind_img = #kind; break;
			TOKENKINDS
#undef X
#undef K
		}

		printf("%s %.*s\n", kind_img, len, l.src + tok.start);
	}
	while (tok.kind != EOI_TK);

	return 0;
}
