#pragma once

#define TOKENKINDS \
	X(EOI) \
	X(PLUS) X(MINUS) \
	X(STAR) X(SLASH) \
	X(ID) X(NUM)

typedef enum {
#define X(kind) kind ## Tk,
	TOKENKINDS
#undef X
} TokenKind;

typedef struct {
	TokenKind kind;
	unsigned long start;
} Token;
