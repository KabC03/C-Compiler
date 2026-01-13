#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H 
#include <stdint.h>



typedef enum INSTRUCTION {

    INVALID = -1,
    ADD,
    SUB,
    MUL,
    DIV,
    DEREF,
    REF,
    EQUAL,
    NOT_EQUAL,
    GREATER_EQUAL,
    LESS_EQUAL,
    GREATER,
    LESS,
    SET_STACK,
    GET_STACK, 

} INSTRUCTION;

extern const char *INSTRUCTION_IMPLEMENTATION[];


#endif
