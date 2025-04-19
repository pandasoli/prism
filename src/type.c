#include "type.h"
#include "node.h"

Type check_type(node) PrismNode **node; {
    PrismNode n = **node;
    switch (n.kind) {
        case BINARY_NK: {
            PrismBinaryNode bin = n.binary;

            Type left = check_type(bin.left);
            Type right = check_type(bin.right);

            if (left.id != right.id) {
                /*
                Type mismatch.
                TODO: write error.
                */

                return (Type) {
                    .id = ERROR_TYPE,
                };
            }

            /* TODO: check edge cases */
            /* Since both types will be equal here, we can return any. */
            return left;
        }

        case UNARY_NK: return check_type(&n.unary);

        case LIT_NK: break;
        case IF_NK: break;
        case ELSE_NK: break;
    }
}

