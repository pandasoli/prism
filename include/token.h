#pragma once

#include <stdlib.h>

#define TOKENKINDS                          \
	X(EOI) X(UNKNOWN)                         \
	                                          \
	/* Arithimatic operators */               \
	X(ADD) X(SUB)                             \
	X(MUL) X(DIV)                             \
	X(MOD)                                    \
	                                          \
	/* Bitwise operators */                   \
	X(BIT_AND) X(BIT_OR)                      \
	X(BIT_SHL) X(BIT_SHR)                     \
	X(BIT_NOT) X(BIT_XOR)                     \
	                                          \
	/* Operators following assignment */      \
	X(ADD_ASSIGN) X(SUB_ASSIGN)               \
	X(MUL_ASSIGN) X(DIV_ASSIGN)               \
	X(MOD_ASSIGN)                             \
	                                          \
	X(BIT_AND_ASSIGN) X(BIT_OR_ASSIGN)        \
	X(BIT_SHL_ASSIGN) X(BIT_SHR_ASSIGN)       \
	X(BIT_NOT_ASSIGN) X(BIT_XOR_ASSIGN)       \
	                                          \
	/* Comparisons */                         \
	X(EQUAL) X(NOT_EQUAL)                     \
	X(LESS) X(GREATER)                        \
	X(LESS_EQUAL) X(GREATER_EQUAL)            \
	                                          \
	/* Logic operators */                     \
	K(LOGIC_AND, "and") K(LOGIC_OR, "or")     \
	K(LOGIC_NOT, "not")                       \
	                                          \
	/* Other symbols */                       \
	X(OPEN_PAREN) X(CLOSE_PAREN)              \
	X(COMMA) X(ASSIGN)                        \
	                                          \
	/* Literals */                            \
	X(IDENT) X(INT) X(FLOAT)                  \
	X(STRING) X(CHAR)

typedef enum {
#define X(kind) kind ## _TK,
#define K(kind, _) kind ## _KW,
	TOKENKINDS
#undef X
#undef K
} TokenKind;

typedef struct {
	TokenKind kind;
	char *img;
	unsigned long len;
} KeywordInfo;

static KeywordInfo keywords[] = {
#define X(kind)
#define K(kind, img) {kind ## _KW, img, sizeof(img) / sizeof(img[0]) - 1},
	TOKENKINDS
#undef X
#undef K
	{0, NULL, 0}
};

typedef struct {
	TokenKind kind;
	unsigned long start;
} Token;
