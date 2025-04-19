#pragma once

#include "token.h"

#define NODEKINDS      \
	X(BINARY) X(UNARY) \
	X(LIT)             \
	                   \
	/* Statements */   \
	X(IF) X(ELSE)

typedef enum {
#define X(kind) kind ## _NK,
	NODEKINDS
#undef X
} PrismNodeKind;

typedef struct PrismNode PrismNode;

typedef struct {
	PrismNode *left;
	PrismNode *right;
	PrismTokenKind op;
} PrismBinaryNode;

typedef struct {
	PrismNode *val;
	PrismTokenKind op;
} PrismUnaryNode;

typedef struct {
	PrismNode *cmp;
	PrismNode *body;
} PrismIfNode;

typedef struct {
	PrismNode *body;
} PrismElseNode;

struct PrismNode {
	PrismNodeKind kind;

	union {
		PrismIfNode if_;
		PrismElseNode else_;
		PrismBinaryNode binary;
		PrismUnaryNode unary;
		PrismToken lit;
	};

	PrismNode *next;
};

