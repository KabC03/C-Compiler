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

    TokenID id;
    char *str;

} Token;

bool tokenise_generate_token_set(void);
Token tokenise_get_next_token(char *str);
void tokenise_destroy_token(Token *token);
void tokenise_destroy_token_set(void);

#endif // TOKENISE_H
