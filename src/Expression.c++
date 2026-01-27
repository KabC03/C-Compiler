#include "Expression.h++"

//LR parsing
//[X + 2 - Y]




uint16_t expression_evaluate(char **text, uint8_t destinationAddress) {
    uint16_t lineCount = 0;
    Token token;
    token = tokenise_consume(text);
    if(token.id != TOKEN_SYMBOL_OPEN_BRACE) {
        printf("Invalid token: '%s'", token.str);
        return false;
    }

    bool expectOperand = true;
    uint8_t operandAddress = 0;
    Instruction instruction;
    instruction.arg2 = 0;
    bool running = true;
    while(running) {
        token = tokenise_consume(text);        
        if(token.id == TOKEN_INVALID) {
            printf("Invalid token: '%s'", token.str);
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
                        printf("Variable '%s' not found\n", token.str);
                        return false;
                    }

                    break;
                } default: {
                    printf("Invalid token: '%s'", token.str);
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
                } case TOKEN_SYMBOL_CLOSE_BRACE: {
                    running = false;
                    break;
                } default: {
                    printf("Invalid token: '%s'", token.str);
                    return false;
                }
            }

            if(operandAddress != 0) {
                instruction.arg1 = destinationAddress;
                instruction.arg2 = operandAddress;
                output_write_instruction(instruction);
                lineCount++;

            } else { //No second operand
                instruction.opcode = INSTRUCTION_IMM;
                instruction.arg1 = destinationAddress;
                instruction.arg2 = 0;
                output_write_instruction(instruction);
                lineCount++;
                break;
            }
        }
        expectOperand = !expectOperand;
    }

    if(expectOperand == true) {
        printf("Expect operand: '%s'", token.str);
        return false; //Last token was a operator
    }

    return lineCount;
}


