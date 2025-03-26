#include <lex.h>
#include <token.h>

#define CURRENT lex->src[lex->pos]
#define NEXT lex->src[++lex->pos]

#define IS_DEC '0' <= CURRENT && CURRENT <= '9'
#define IS_ID ('a' <= CURRENT && CURRENT <= 'z') || ('A' <= CURRENT && CURRENT <= 'Z') || CURRENT == '_'

int lex(lex, token) Lex *lex; Token *token; {
	while (
		CURRENT == ' ' ||
		CURRENT == '\t' ||
		CURRENT == '\r' ||
		CURRENT == '\n'
	) NEXT;

	unsigned kind = EOITk;
	unsigned long start = lex->pos;

	switch (CURRENT) {
		case 0: NEXT; break;

		case '+': kind = PLUSTk; NEXT; break;
		case '-': kind = MINUSTk; NEXT; break;
		case '*': kind = STARTk; NEXT; break;
		case '/': kind = SLASHTk; NEXT; break;

		default:
			if (IS_DEC) {
				kind = NUMTk;
				while (IS_DEC) NEXT;
			}

			else if (IS_ID) {
				kind = IDTk;
				while (IS_ID || IS_DEC) NEXT;
			}
	}

	*token = (Token) { kind, start };
	return 0;
}
