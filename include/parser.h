#pragma once

#include <lexer.h>
#include <node.h>

typedef struct {
	PrismToken current;
	PrismLexer *lexer;
} PrismParser;

extern prism_parse(PrismParser *, PrismNode **);
