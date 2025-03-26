#include <stdio.h>
#include <lex.h>
#include <token.h>


int main(void) {
	Lex l = { "12 +\r4 /\n5 * asd\t-1", 0 };
	Token tok;
	int err;

	do {
		err = lex(&l, &tok);
		if (err) break;
		printf("%d %ld\n", tok.kind, tok.start);
	}
	while (tok.kind != EOITk);

	return 0;
}
