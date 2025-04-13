#pragma once

typedef enum {
#define T(type) TYPE_ ## type
    T(INT), T(FLOAT),
    T(STR), T(CHAR),
    T(BOOL), T(VOID),
#undef T
} TypeId;

typedef struct {
    TypeId id;
    /* union { }; // TODO: for types that require more information */
} Type;
