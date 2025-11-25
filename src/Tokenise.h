#ifndef TOKENISE_H
#define TOKENISE_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Vector.h"
#include "Map.h"
#include "Tokens.h"

typedef struct Token {

    TOKEN_ID id;
    char str[MAX_TOKEN_LENGTH];

} Token;


bool tokenise_generate_token_set(void);
void tokenise_destroy_token_set(void);
Token tokenise_peek(char **str);
Token tokenise_consume(char **str);

#endif // TOKENISE_H
