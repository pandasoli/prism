#pragma once

#include <token.h>

#define NODEKINDS    \
	X(BINARY) X(UNARY) \
	X(LIT)

typedef enum {
#define X(kind) kind ## _NK,
	NODEKINDS
#undef X
} NodeKind;

typedef struct Node Node;

typedef struct {
	Node *left;
	Node *right;
	TokenKind op;
} BinaryNode;

typedef struct {
	Node *val;
	TokenKind op;
} UnaryNode;

struct Node {
	NodeKind kind;

	union {
		BinaryNode binary;
		UnaryNode unary;
		Token lit;
	};

	Node *next;
};
