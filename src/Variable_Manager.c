#include "Variable_Manager.h"
const uint8_t numVariables = 10;

Variable stack[numVariables]; //Stack memory


uint8_t variable_manager_add(Variable variable, int8_t value) {
    static uint8_t top = 0; //0 address is NULL (used for temporary storage for constants)

    Instruction instruction;
    instruction.arg2 = 0;
    if(top > numVariables - 1) {
        //Full stack
        return false;
    }
    top++;
    stack[top] = variable;

    instruction.opcode = INSTRUCTION_SET;
    instruction.arg1 = value;
    instruction_write(instruction);
    return top;
}


uint8_t variable_manager_get(char *name) {


    static uint8_t pushed = 0;
    for(size_t i = 1; i < numVariables + 1; i++) {
        Variable var = stack[i - 1];
        if(strlen(name) == strlen(var.name)) {
            if(strcmp(name, var.name) == 0) {
                return i; //i is the address
            }
        }
    }

    return 0; //Not found
}



