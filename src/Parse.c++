#include "Parse.h++"

std::vector<std::tuple<std::string, uint16_t>> labels; //Label, Address
uint16_t lineCount = 0; //Counting instructions
uint16_t retAddress = 0; //Return address for goto

std::vector<uint16_t> ifStack;
uint16_t ifCount = 0;

bool internal_parse_label(char **text) {
    //label x
    Token label = tokenise_consume(text);
    if(label.id == TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s'", label.str);
        return false;
    }

    for(auto item : labels) {
        if(std::get<0>(item) == label.str) {
            //Duplicate label
            printf("Duplicated label '%s'\n", label.str);
            return false;
        }
    }

    labels.push_back(std::make_tuple(label.str, lineCount));

    return true;
}

bool internal_parse_goto(char **text) {
    //goto x
    retAddress = lineCount;
    
    Token label = tokenise_consume(text);
    if(label.id == TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s'", label.str);
        return false;
    }

    for(auto item : labels) {
        if(std::get<0>(item) == label.str) {

            Instruction instruction;
            instruction.opcode = INSTRUCTION_GOTO;
            instruction.arg1 = (int)std::get<1>(item);
            instruction.arg2 = 0;
            output_write_instruction(instruction);
            lineCount++;

            return true;
        }
    }

    printf("Label '%s' not found\n", label.str);
    return false;
}

bool internal_parse_let(char **text) {
    //set x = (1 + 2 + z + var)

    Token varName = tokenise_consume(text);
    if(varName.id != TOKEN_IDENTIFIER) {
        printf("Invalid token: '%s'\n", varName.str);
        return false;
    }

    Token equals = tokenise_consume(text);
    if(equals.id != TOKEN_SYMBOL_EQUAL) {
        printf("Expect equals\n");
        return false;
    }

    Token immediate = tokenise_consume(text);
    if(varName.id != TOKEN_LITERAL) {
        printf("Invalid immediate: '%s'\n", varName.str);
        return false;
    }

    uint16_t returnAddress = variable_manager_add(varName.str, std::stoi(immediate.str));
    if(returnAddress == 0) {
        return false;
    }

    //Expression part
    uint16_t lineCountAdd = expression_evaluate(text, returnAddress);
    lineCount += lineCountAdd;
    if(lineCountAdd == 0) {
        printf("Invalid expression\n");
        return false;
    }

    return true;
}

bool internal_parse_ret(char **text) {

    Instruction instruction;
    instruction.opcode = INSTRUCTION_GOTO;
    instruction.arg1 = retAddress;
    instruction.arg2 = 0;
    output_write_instruction(instruction);
    lineCount++;

    return true;
}

bool internal_parse_if(char **text) {
    
}

