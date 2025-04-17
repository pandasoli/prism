#include "node.h"

check_type(node) PrismNode **node; {
    switch ((**node).kind) {
        case BINARY_NK: break;
        case UNARY_NK: break;
        case LIT_NK: break;
        case IF_NK: break;
        case ELSE_NK: break;
    }
}

