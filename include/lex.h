#pragma once

#include <token.h>

typedef struct {
	char *src;
	unsigned long pos;
} Lex;

extern int lex(Lex *, Token *);
