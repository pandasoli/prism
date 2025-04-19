#pragma once

#include "node.h"

#define TYPEIDS     \
	X(INT) X(FLOAT) \
	X(STR) X(CHAR)  \
	X(BOOL) X(VOID) \
    \
    X(ERROR) /* when the type checker fails */

typedef enum {
#define X(type) type ## _TYPE,
	TYPEIDS
#undef X
} TypeId;

typedef struct {
    TypeId id;
    /* union { }; // TODO: for types that require more information */
} Type;

extern Type check_type(PrismNode **);

