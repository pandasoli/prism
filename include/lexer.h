#pragma once

#include <token.h>

typedef struct {
	char *src;
	unsigned long pos;
} Lexer;

extern lex(Lexer *, Token *);
