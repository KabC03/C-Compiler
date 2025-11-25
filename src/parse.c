#include "Parse.h"

typedef struct Variable {
    Token name;
    size_t stackAddress;
} Variable;

typedef struct Function {
    Token name;
    size_t address;
} Function;

typedef struct Scope {
    size_t elements;
    Variable variables[]; //Faster than a map for small number of variables
} Scope;

typedef struct Functions {
    size_t elements;
    Function functions[];
} Functions;

bool parse_parse(FILE *inp, FILE *out) {

    fseek(inp, 0, SEEK_END);
    long inpSize = ftell(inp);
    if(size < 0) {
        return false;
    }
    rewind(inp);
    char *buffer = malloc(sizeof(char) * (inpSize + 1));
    if(!buffer) {
        return false;
    }
    size_t read = fread(buffer, sizeof(char), MAX_TOKEN_LENGTH, inp);
    buffer[read] = '\0';

    Token token = {TOKEN_IDENTIFIER, '\0'};

    while(token.id != TOKEN_INVALID) {
        token = tokenise_peek(&buffer);
        switch(token.id) {
            case TOKEN_INVALID: {
                return false;

            } case TOKEN_DEF: { //Function declaration

                break;
            } case TOKEN_IDENTIFIER: { //Variable reassignment

                break;
            } case TOKEN_LET: { //Variable assignment

                break;
            } case TOKEN_IF: { //If statement

                break;
            } case TOKEN_WHILE: { //White statement

                break;
            } case TOKEN_RETURN: { //Return statement

                break;
            } case default: {

                return false;
            }
        }
    }

    return true;
}


