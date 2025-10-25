#ifndef TOKENISE_H
#define TOKENISE_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Vector.h"
#include "Hashmap.h"
#include "Tokens.h"


typedef struct Token {

    TokenID id;
    char *str;

} Token;





#endif // TOKENISE_H
