#pragma once

#include <token.h>

typedef struct {
	char *src;
	unsigned long pos;
} PrismLexer;

extern prism_lex(PrismLexer *, PrismToken *);

