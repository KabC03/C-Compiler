#include "Tokenise.h++"

uint16_t lineNumber = 0;

static inline TOKEN_ID internal_find(char *str, uint8_t size) {
    TOKEN_ID id = TOKEN_INVALID; //Error condition
    for(uint8_t i = 0; i < TOKEN_VECTOR_SIZE; i++) {
        if(size == strlen(TOKEN_VECTOR[i])) {
            if(memcmp(str, TOKEN_VECTOR[i], size) == 0) {
                return (TOKEN_ID)i;
            }
        }
    }
    return id;
}

static inline Token internal_tokenise(char **inp, bool consume) {

    static Token token; //Cache last token
    static bool wasPeeked = false;
    if(consume && wasPeeked) { //If the token was previously peeked, and its consumed then return
        wasPeeked = false;
        return token;
    } 
    if(!consume) {
        wasPeeked = true;
    }

    char *str = *inp;
    while(isspace(*str)) { //Skip leading whitespace
        if(*str == '\n') {
            lineNumber++;
        }

        str++;
    }
    int16_t idx = 0;
    bool containsSymbols = false;
    bool containsLetters = false;
    while(1) {
        if(str[idx] == '\0') { //Incomplete token
            memcpy(token.str, str, idx + 1);
            token.id = TOKEN_INVALID;
            return token;
        } else if(idx > MAX_TOKEN_LENGTH - 2) {
            memcpy(token.str, str, MAX_TOKEN_LENGTH);
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

            TOKEN_ID tokenType = internal_find(str, idx + 1);

            if(tokenType != -1) {
                //Keyword found
                token.id = tokenType;

                //Bug testing
                //memcpy(token.str, str, idx + 1);
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
        *inp += (idx + 2);
    }

    return token;
}


Token tokenise_peek(char **str) {
    return internal_tokenise(str, false);
}


Token tokenise_consume(char **str) {
    return internal_tokenise(str, true);
}

uint16_t get_line_number(void) {
    return lineNumber;
}






