#include "Expression.h"


bool internal_parse_expression(Variable *variables, size_t numVariables, char **inp) { //Left to right evaluation

    while(1) {
        Token operand = tokenise_consume(inp);
\


        Token operator = tokenise_consume(inp);
        switch(operator.id) {
            case TOKEN_SYMBOL_PLUS: {

            } case TOKEN_SYMBOL_MINUS: {

            } case TOKEN_SYMBOL_ASTERISK: {

            } case TOKEN_SYMBOL_SLASH: {

            } default: {

            }
        }
    }

    return true;
}




