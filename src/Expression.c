#include "Expression.h"

//LR parsing
//[X + 2 - Y]



bool expression_evaluate(char *text, uint8_t destinationAddress) {

    Token token;
    token = tokenise_consume(&text);
    if(token.id != TOKEN_SYMBOL_OPEN_BRACE) {
        return false;
    }

    bool expectOperand = true;
    while(token.id != TOKEN_SYMBOL_CLOSE_BRACE && !expectOperand) {
        token = tokenise_consume(&text);
        if(token.id == TOKEN_INVALID) {
            return false;
        }

        if(expectOperand) {
            switch(token.id) {
                case TOKEN_LITERAL: {
                    break;
                } case TOKEN_IDENTIFIER: {
                    break;
                } default: {
                    return false;
                }
            }

        } else {
            switch(token.id) {
                case TOKEN_SYMBOL_PLUS: {
                    break;
                } case TOKEN_SYMBOL_MINUS: {
                    break;
                } case TOKEN_SYMBOL_ASTERISK: {
                    break;
                } case TOKEN_SYMBOL_SLASH: {
                    break;
                } default: {
                    return false;
                }
            }
        }
    }

    return true;
}


