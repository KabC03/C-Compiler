#include "Variable_Manager.h"
const uint8_t numVariables = 10;

Variable stack[numVariables]; //Stack memory


Instruction variable_manager_add(Variable variable, int8_t value) {
    static uint8_t top = 0; //0 address is NULL

    Instruction instruction;
    if(top > numVariables - 1) {
        //Full stack
        instruction.opcode = INVALID;
        return instruction;
    }
    top++;
    stack[top] = variable;

    instruction.opcode = SET_STACK;
    instruction.arg1 = value;
    return instruction;
}


Instruction variable_manager_get(char *name) {


    static uint8_t pushed = 0;
    for(size_t i = 1; i < numVariables + 1; i++) {
        Variable var = stack[i - 1];
        if(strlen(name) == strlen(var.name)) {
            if(strcmp(name, var.name) == 0) {
                Instruction instruction;
                instruction.opcode = GET_STACK;
                instruction.arg1 = i;
                return instruction; //i is the address
            }
        }
    }

    Instruction instruction;
    instruction.opcode = INVALID;
    return instruction; //Not found
}



