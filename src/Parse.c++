#include "Parse.h++"

std::vector<std::string> labels; //Label
uint16_t retAddress = 0; //Return address for goto

std::vector<uint16_t> ifStack;
uint16_t ifStatementCount = 0;

bool internal_parse_label(char **text) {
    //label x
    Token label = tokenise_consume(text);
    if(label.id == TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s', %d", label.str, label.id);
        return false;
    }

    for(auto item : labels) {
        if(item == label.str) {
            //Duplicate label
            printf("Duplicated label '%s'\n", label.str);
            return false;
        }
    }

    Instruction instruction;
    instruction.opcode = INSTRUCTION_LABEL;
    instruction.arg3 = label.str;
    instruction.arg1 = 0;
    instruction.arg2 = 0;
    output_write_instruction(instruction);
    

    labels.push_back(label.str);

    return true;
}

bool internal_parse_goto(char **text) {
    //goto x
    
    Token label = tokenise_consume(text);
    if(label.id == TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s', %d", label.str, label.id);
        return false;
    }

    for(auto item : labels) {
        if(item == label.str) {

            Instruction instruction;
            instruction.opcode = INSTRUCTION_GOTO;
            instruction.arg3 = item;
            instruction.arg1 = 0;
            instruction.arg2 = 0;
            output_write_instruction(instruction);
            

            return true;
        }
    }

    printf("Label '%s' not found\n", label.str);
    return false;
}

bool internal_parse_let(char **text) {
    //let x = (1 + 2 + z + var)

    Token varName = tokenise_consume(text);
    if(varName.id != TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s', '%d'\n", varName.str, varName.id);
        return false;
    }

    Token equals = tokenise_consume(text);
    if(equals.id != TOKEN_SYMBOL_EQUAL) {
        printf("Expect equals\n");
        return false;
    }

    uint16_t returnAddress = variable_manager_add(varName.str);
    if(returnAddress == 0) {
        return false;
    }

    //Expression part
    if(!expression_evaluate(text, returnAddress)) {
        printf("Invalid expression\n");
        return false;
    }

    return true;
}

bool internal_parse_ret(void) {
    //ret
    Instruction instruction;
    instruction.opcode = INSTRUCTION_GOTO;
    instruction.arg1 = retAddress;
    instruction.arg2 = 0;
    output_write_instruction(instruction);
    

    return true;
}

bool internal_parse_if(char **text) {
    //if x == y
    //...
    //end
    Token var1 = tokenise_consume(text);
    if(var1.id == TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s', %d", var1.str, var1.id);
        return false;
    }

    Instruction instruction;
    Token op = tokenise_consume(text);
    switch(op.id) {
        case TOKEN_SYMBOL_DOUBLE_EQUAL: {
            instruction.opcode = INSTRUCTION_EQUAL;
            break;
        } case TOKEN_SYMBOL_NOT_EQUAL: {
            instruction.opcode = INSTRUCTION_NOT_EQUAL;
            break;
        } case TOKEN_SYMBOL_GREATER_EQUAL: {
            instruction.opcode = INSTRUCTION_GREATER_EQUAL;
            break;
        } case TOKEN_SYMBOL_GREATER_THAN: {
            instruction.opcode = INSTRUCTION_GREATER;
            break;
        } default: {
            printf("Invalid token: '%s', '%d'", op.str, op.id);
            return false;
            break;
        }
    }

    Token var2 = tokenise_consume(text);
    if(var2.id == TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s', '%d'", var2.str, var2.id);
        return false;
    }

    uint8_t var1Address = variable_manager_get(var1.str);
    uint8_t var2Address = variable_manager_get(var2.str);

    if(var1Address == 0) {
        printf("Invalid variable: '%s'", var1.str);
        return false;
    }
    if(var2Address == 0) {
        printf("Invalid variable: '%s'", var2.str);
        return false;
    }


    instruction.arg1 = var1Address;
    instruction.arg2 = var2Address;
    ifStack.push_back(ifStatementCount);
    instruction.arg3 = "IF_" + std::to_string(ifStatementCount);
    ifStatementCount++;
    return true;
}

bool internal_parse_end(void) {
    if(ifStack.size() == 0) {
        printf("Expect if statement\n");
        return false;
    }
    Instruction instruction;
    instruction.opcode = INSTRUCTION_LABEL;

    uint16_t val = ifStack.back();
    ifStack.pop_back();
    instruction.opcode = INSTRUCTION_GOTO;
    instruction.arg3 = "IF_" + std::to_string(val);
    instruction.arg1 = 0;
    instruction.arg2 = 0;
    output_write_instruction(instruction);
    

    return true;
}

bool parse_parse(char **text) {


    while(tokenise_peek(text).id != TOKEN_EOF) {
        Token token = tokenise_consume(text);

        switch(token.id) {
            case TOKEN_KEYWORD_LET: {
                if(internal_parse_let(text) == false) {

                    return false;
                }

                break;
            } case TOKEN_KEYWORD_END: {
                if(internal_parse_end() == false) {

                    return false;
                }

                break;
            } case TOKEN_KEYWORD_GOTO: {
                if(internal_parse_goto(text) == false) {
                    return false;
                }

                break;
            } case TOKEN_KEYWORD_IF: {
                if(internal_parse_if(text) == false) {

                    return false;
                }

                break;
            } case TOKEN_KEYWORD_LABEL: {
                if(internal_parse_label(text) == false) {

                    return false;
                }

                break;
            } case TOKEN_KEYWORD_RET: {
                if(internal_parse_ret() == false) {

                    return false;
                }

                break;
            } default: {
                printf("Invalid token '%s', '%d'\n", token.str, token.id);
                return false;
                break;
            }
        }
    }

    return true;
}

