#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <string>
#include "Tokens.h++"
#include "Instructions.h++"

typedef struct Token {

    TOKEN_ID id;
    char str[MAX_TOKEN_LENGTH];

} Token;


typedef struct Instruction {

    INSTRUCTION opcode = INSTRUCTION_INVALID;
    uint8_t arg1 = 0;
    uint8_t arg2 = 0;
    std::string arg3 = "";

} Instruction;


#endif
