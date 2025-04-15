#include <stdio.h>
#include <stdlib.h>
#include <lexer.h>
#include <string.h>
#include <token.h>

#define CURRENT lex->src[lex->pos]
#define NEXT lex->src[++lex->pos]

#define IS_DEC '0' <= CURRENT && CURRENT <= '9'
#define IS_ID ('a' <= CURRENT && CURRENT <= 'z') || ('A' <= CURRENT && CURRENT <= 'Z') || CURRENT == '_'

lex(lex, token) Lexer *lex; Token *token; {
	while (
		CURRENT == ' ' ||
		CURRENT == '\t' ||
		CURRENT == '\r' ||
		CURRENT == '\n'
	) NEXT;

	TokenKind kind = UNKNOWN_TK;
	unsigned long start = lex->pos;

	switch (CURRENT) {
		case 0: kind = EOI_TK; NEXT; break;

		case '(': kind = OPEN_PAREN_TK; NEXT; break;
		case ')': kind = CLOSE_PAREN_TK; NEXT; break;
		case '{': kind = OPEN_BRACE_TK; NEXT; break;
		case '}': kind = CLOSE_BRACE_TK; NEXT; break;
		case ',': kind = COMMA_TK; NEXT; break;

		case '+':
			if (NEXT == '=') { kind = ADD_ASSIGN_TK; NEXT; }
			else kind = ADD_TK;
		break;

		case '-':
			if (NEXT == '=') { kind = SUB_ASSIGN_TK; NEXT; }
			else kind = SUB_TK;
		break;

		case '*':
			if (NEXT == '=') { kind = MUL_ASSIGN_TK; NEXT; }
			else kind = MUL_TK;
		break;

		case '/':
			if (NEXT == '=') { kind = DIV_ASSIGN_TK; NEXT; }
			else kind = DIV_TK;
		break;

		case '%':
			if (NEXT == '=') { kind = MOD_ASSIGN_TK; NEXT; }
			else kind = MOD_TK;
		break;

		case '&':
			if (NEXT == '=') { kind = BIT_AND_ASSIGN_TK; NEXT; }
			else kind = BIT_AND_TK;
		break;

		case '|':
			if (NEXT == '=') { kind = BIT_OR_ASSIGN_TK; NEXT; }
			else kind = BIT_OR_TK;
		break;

		case '^':
			if (NEXT == '=') { kind = BIT_XOR_ASSIGN_TK; NEXT; }
			else kind = BIT_XOR_TK;
		break;

		case '~':
			if (NEXT == '=') { kind = BIT_NOT_ASSIGN_TK; NEXT; }
			else kind = BIT_NOT_TK;
		break;

		case '=':
			kind = ASSIGN_TK;
			NEXT;

			if      (CURRENT == '=') { kind = EQUAL_TK; NEXT; }
			else if (CURRENT == '<') { kind = LESS_EQUAL_TK; NEXT; }
			else if (CURRENT == '>') { kind = GREATER_EQUAL_TK; NEXT; }
		break;

		case '!':
			if (NEXT == '=') {
				kind = NOT_EQUAL_TK;
				NEXT;
			}
		break;

		case '<': {
			kind = LESS_TK;
			NEXT;

			if      (CURRENT == '=') { kind = LESS_EQUAL_TK; NEXT; }
			else if (CURRENT == '<') {
				kind = BIT_SHL_TK;
				NEXT;
				if (CURRENT == '=') { kind = BIT_SHL_ASSIGN_TK; NEXT; }
			}
		} break;

		case '>': {
			kind = GREATER_TK;
			NEXT;

			if      (CURRENT == '=') { kind = GREATER_TK; NEXT; }
			else if (CURRENT == '>') {
				kind = BIT_SHR_TK;
				NEXT;
				if (CURRENT == '=') { kind = BIT_SHR_ASSIGN_TK; NEXT; }
			}
		} break;

		case '\'': {
			kind = CHAR_TK;

			if (NEXT != '\'') {
				kind = STRING_TK;
				while (NEXT != '\'');
				NEXT;
			}
		} break;

		default: {
			if (IS_DEC) {
				kind = INT_TK;
				while (IS_DEC) NEXT;

				if (CURRENT == '.') {
					kind = FLOAT_TK;
					NEXT;
					while (IS_DEC) NEXT;
				}
			}

			else if (IS_ID) {
				kind = IDENT_TK;
				while (IS_ID || IS_DEC) NEXT;

				/* Keywords */
				int i;
				char *src = lex->src + start;
				unsigned long len = lex->pos - start;

				for (i = 0; keywords[i].kind != 0 && kind == IDENT_TK; ++i)
					if (keywords[i].len == len && strncmp(keywords[i].img, src, len) == 0) {
						kind = keywords[i].kind;
						break;
					}
			}
		}
	}

	if (kind == UNKNOWN_TK) {
		fprintf(stderr, "Found unknown token at position %ld: %c\n", start, CURRENT);
		exit(1);
	}

	*token = (Token) { kind, start };
	return 0;
}
