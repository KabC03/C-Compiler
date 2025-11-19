#include "Tokenise.h"



Hashmap tokenSet;
bool tokenise_generate_token_set(void) {
    if(hashmap_init(&tokenSet, sizeof(char*), sizeof(int), 10) == false) {
        return false;
    }

    for(size_t i = 0; i < TOKEN_VECTOR_SIZE; i++) {
        if(hashmap_insert(&tokenSet, TOKEN_VECTOR[i], &i) == false) {
            hashmap_destroy(&tokenSet);
            return false;
        }
    }

    return true;
}

Token tokenise_get_next_token(char *str) { //Get the next token from a string segment

    Token token = {TOKEN_INVALID, NULL};
    token.str = calloc(sizeof(char), (strlen(str) + 1));

    bool isNumericLiteral = isdigit(str[0]); //Number literals start with a number
    for(size_t i = 0; i < strlen(str); i++) {
        token.str[i] = str[i];
 
        if(ispunct(str[i]) != ispunct(str[i + 1]) ||
            isspace(str[i + 1]) == true) { //Stop tokenising on space or symbol boundary
            break;
        }
    }

    int *tokenID = (int*)hashmap_find(&tokenSet, token.str);
    if(tokenID == NULL) { //Not a keyword
        if(isNumericLiteral == true) {
            token.id = TOKEN_LITERAL;
        } else {
            token.id = TOKEN_IDENTIFIER;
        }
    } else { //Keyword
        token.id = *tokenID;
    }

    return token;
}


void tokenise_destroy_token(Token *token) {
    free(token->str);
    return;
}


void tokenise_destroy_token_set(void) {
    hashmap_destroy(&tokenSet);
    return;
}


