#include "parse.h"



typedef struct Variable {
    char *name;
    Token type;
    uint64_t value;

    long registerIndex;
    long stackOffset;

} Variable;

typedef struct Scope {
    Vector variables;
} Scope;






