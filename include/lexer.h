#pragma once

#include <token.h>

typedef struct {
	char *src;
	unsigned long pos;
} Lexer;

extern int lex(Lexer *, Token *);
