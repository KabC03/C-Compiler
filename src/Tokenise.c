#include "Tokenise.h"


Map tokenSet;
bool tokenise_generate_token_set(void) {

    if(!map_init(&tokenSet, 10)) {
        return false;
    }

    for(size_t i = 0; i < TOKEN_VECTOR_SIZE; i++) {
        if(!map_insert(&tokenSet, TOKEN_VECTOR[i], strlen(TOKEN_VECTOR[i]), &i, sizeof(size_t))) {
            map_destroy(&tokenSet);
            return false;
        }
    }

    return true;
}

void tokenise_destroy_token_set(void) {
    map_destroy(&tokenSet);
    return;
}


Token internal_tokenise(char **inp, bool consume) {
    Token token;
    char *str = *inp;
    while(isspace(*str)) { //Skip leading whitespace
        str++;
    }
    size_t idx = 0;
    bool containsSymbols = false;
    bool containsLetters = false;
    while(1) {
        if(str[idx] == '\0') { //Incomplete token
            token.id = TOKEN_INVALID;
            return token;
        } else if(idx > MAX_TOKEN_LENGTH - 2) {
            token.id = TOKEN_INVALID;
            return token;
        }

        if(ispunct(str[idx])) {
            containsSymbols = true; //Symbolic token
        } else if(isalpha(str[idx])) {
            containsLetters = true;
        }

        //End of token check
        if((ispunct(str[idx + 1]) != containsSymbols) || 
            isspace(str[idx + 1]) ||
            str[idx + 1] == '\0') {
            //Token can only contain symbols or no symbols
            //End of token when a space occurs
            TOKEN_ID *tokenType = (TOKEN_ID*)map_find(&tokenSet, str, idx + 1);
            if(tokenType) {
                //Keyword found
                token.id = *tokenType;

                //Bug testing
                memcpy(token.str, str, idx + 1);
                token.str[idx + 1] = '\0';
                break;
            } else if(containsSymbols == false && containsLetters == false) {
                //Must be true for numeric literals
                token.id = TOKEN_LITERAL;
                memcpy(token.str, str, idx + 1);
                token.str[idx + 1] = '\0';
                break;
            } else {
                //Identifier
                memcpy(token.str, str, idx + 1);
                token.id = TOKEN_IDENTIFIER;
                token.str[idx + 1] = '\0';
                break;
            }
        }
        idx++;
    }


    if(consume) {
        *inp += (idx + 1);
    }

    return token;
}


Token tokenise_peek(char **str) {
    return internal_tokenise(str, false);
}


Token tokenise_consume(char **str) {
    return internal_tokenise(str, true);
}







