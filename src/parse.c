#include "parse.h"
#define NUM_REGISTERS 4

typedef struct Variable {
    Token name;
    size_t stackAddress;
} Variable;

typedef struct Scope {
    Vector variables; //Faster than a map for small number of variables
} Scope;





