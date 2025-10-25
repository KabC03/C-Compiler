#ifndef TOKENISE_H
#define TOKENISE_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "Tokens.h"


typedef struct Token {

    TokenID id;
    char *str;

} Token;



Token tokenise_get_next_token(char *str) { //Get the next token from a string segment

    Token token = {TOKEN_INVALID, NULL};

    Vector buffer;
    vector_init(&buffer, sizeof(char));

    for(size_t i = 0; i < strlen(str); i++) {
        
    }

    return token;
}



#endif // TOKENISE_H
