#pragma once

#define TYPEIDS   \
	X(INT) X(FLOAT) \
	X(STR) X(CHAR)  \
	X(BOOL) X(VOID)

typedef enum {
#define X(type) type ## _TYPE,
	TYPEIDS
#undef X
} TypeId;

typedef struct {
    TypeId id;
    /* union { }; // TODO: for types that require more information */
} Type;
