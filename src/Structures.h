#ifndef STRUCTURES_H
#define STRUCTURES_H
#include "Tokens.h"
#include "Instructions.h"

typedef struct Token {

    TOKEN_ID id;
    char str[MAX_TOKEN_LENGTH];

} Token;

typedef struct Variable {
    char name[MAX_TOKEN_LENGTH]
} Variable;

typedef struct Instruction {

    INSTRUCTION opcode;
    uint8_t arg1;
    uint8_t arg2;
    int16_t arg3;
    char str[MAX_TOKEN_LENGTH];

} Instruction;


#endif
