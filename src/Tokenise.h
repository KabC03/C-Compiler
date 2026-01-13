#ifndef TOKENISE_H
#define TOKENISE_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Tokens.h"
#include "Structures.h"




Token tokenise_peek(char **str);
Token tokenise_consume(char **str);

/*
bool tokenise_generate_token_set(void);
void tokenise_destroy_token_set(void);
*/

#endif // TOKENISE_H
