#pragma once

#include <token.h>

#define NODEKINDS    \
	X(BINARY) X(UNARY) \
	X(LIT)             \
	                   \
	/* Statements */   \
	X(IF) X(ELSE)

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

typedef struct {
	Node *cmp;
	Node *body;
} IfNode;

typedef struct {
	Node *body;
} ElseNode;

struct Node {
	NodeKind kind;

	union {
		IfNode if_;
		ElseNode else_;
		BinaryNode binary;
		UnaryNode unary;
		Token lit;
	};

	Node *next;
};
