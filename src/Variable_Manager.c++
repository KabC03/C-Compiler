#include "Variable_Manager.h++"

std::vector<std::string> stack;

uint16_t variable_manager_add(std::string variable, int8_t value) {

    for(std::string var : stack) {
        if(var == variable) {
            return 0; //Duplicate variable
        }
    }

    Instruction instruction;
    instruction.arg2 = 0;
    stack.push_back(variable);

    instruction.opcode = INSTRUCTION_SET;
    instruction.arg1 = value;
    output_write_instruction(instruction);
    return stack.size();
}


uint8_t variable_manager_get(char *name) {


    for(size_t i = 1; i < stack.size() + 1; i++) {
        
        std::string var = stack[i - 1];
        if(var == name) {
            return i; //Address
        }
    }

    return 0; //Not found
}



