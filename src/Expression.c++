#include "Expression.h++"

//LR parsing
//[X + 2 - Y]




bool expression_evaluate(char **text, uint8_t destinationAddress) {
    Token token;
    token = tokenise_consume(text);
    if(token.id != TOKEN_SYMBOL_OPEN_BRACE) {
        printf("Invalid tokens: '%s'\n", token.str);
        return false;
    }
    bool running = true;
    bool expectOperand = true;
    uint16_t loaded = 0;
    Instruction instruction;

    while(running) {
        token = tokenise_consume(text);
        if(expectOperand) {
            if(loaded == 0) {
                instruction.arg1 = destinationAddress;
            } else {
                instruction.arg1 = destinationAddress;
            }

            loaded++;
            switch(token.id) {
                //Load addition into zero address


                case TOKEN_LITERAL: {
                    instruction.arg3 = token.str;
                    instruction.opcode = INSTRUCTION_IMM;
                    output_write_instruction(instruction);
                    break;
                } case TOKEN_IDENTIFIER: {
                    auto varAddress = variable_manager_get(token.str);
                    if(varAddress == 0) {
                        printf("Unknown variable: '%s'\n", token.str);
                        return false;
                    }
                    instruction.arg3 = varAddress;
                    instruction.opcode = INSTRUCTION_MOV;
                    output_write_instruction(instruction);

                    break;
                } default: {
                    printf("Expect valid operand '%s'\n", token.str);
                    return false;
                    break;
                }
            }
        } else {

            switch(token.id) {
                case TOKEN_SYMBOL_CLOSE_BRACE: { //End condition
                    running = false;
                } case TOKEN_SYMBOL_PLUS: {
                    instruction.opcode = INSTRUCTION_ADD;
                    break;
                } case TOKEN_SYMBOL_MINUS: {
                    instruction.opcode = INSTRUCTION_SUB;
                    break;
                } case TOKEN_SYMBOL_SLASH: {
                    instruction.opcode = INSTRUCTION_DIV;
                    break;
                } case TOKEN_SYMBOL_ASTERISK: {
                    instruction.opcode = INSTRUCTION_MUL;
                    break;
                } default: {
                    printf("Expect valid operator '%s'\n", token.str);
                    return false;
                    break;
                }
            }

            if(loaded >= 2) {
                instruction.arg1 = destinationAddress;
                instruction.arg2 = 0; //Source address always zero

                output_write_instruction(instruction);
            }

        }

        expectOperand = !expectOperand;
    }

    if(!expectOperand) { //Condition is inverted above
        printf("Expect operand after operator\n");
        return false;
    }

    return true;
}


