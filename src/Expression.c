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
    uint8_t operandAddress = 0;
    Instruction instruction;
    instruction.arg2 = 0;
    instruction.str[0] = '\0';
    while(token.id != TOKEN_SYMBOL_CLOSE_BRACE && !expectOperand) {
        token = tokenise_consume(&text);        
        if(token.id == TOKEN_INVALID) {
            return false;
        }

        if(expectOperand) {
            switch(token.id) {
                case TOKEN_LITERAL: {
                    //Load to temporary register 0
                    instruction.opcode = INSTRUCTION_SET;
                    instruction.arg1 = atoi(token.str);
                    output_write_instruction(instruction);

                    operandAddress = 0;
                    break;
                } case TOKEN_IDENTIFIER: {
                    //Load from stack

                    operandAddress = variable_manager_get(token.str);
                    if(operandAddress == 0) {
                        return false;
                    }

                    break;
                } default: {
                    return false;
                }
            }


        } else {
            switch(token.id) {
                case TOKEN_SYMBOL_PLUS: {
                    instruction.opcode = INSTRUCTION_ADD;
                    break;
                } case TOKEN_SYMBOL_MINUS: {
                    instruction.opcode = INSTRUCTION_SUB;
                    break;
                } case TOKEN_SYMBOL_ASTERISK: {
                    instruction.opcode = INSTRUCTION_MUL;
                    break;
                } case TOKEN_SYMBOL_SLASH: {
                    instruction.opcode = INSTRUCTION_DIV;
                    break;
                } default: {
                    return false;
                }
            }
            instruction.arg1 = destinationAddress;
            instruction.arg2 = operandAddress;
            output_write_instruction(instruction);
        }
        expectOperand = !expectOperand;
    }

    if(expectOperand == true) {
        return false; //Last token was a operator
    }

    return true;
}


