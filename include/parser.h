#pragma once

#include <lexer.h>
#include <node.h>

typedef struct {
	Token	current;
	Lexer *lexer;
} Parser;

extern parse(Parser *, Node **);
