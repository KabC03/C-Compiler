#include "Tokens.h"

const char *TOKEN_VECTOR[] = {
    "GOTO",
    "LABEL",
    "LET",
    "IF",
    "END",
    "LOAD",

    "+",
    "-",
    "*",
    "/",

    "=",

    "==",
    "!=",
    ">",
    ">=",

    ";",

    "[",
    "]",
};

const size_t TOKEN_VECTOR_SIZE = sizeof(TOKEN_VECTOR) / sizeof(TOKEN_VECTOR[0]);



