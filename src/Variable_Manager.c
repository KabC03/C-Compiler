#include "Variable_Manager.h"
#define NUM_VARIABLES 10

Variable stack[NUM_VARIABLES]; //Stack memory


uint8_t variable_manager_add(Variable variable, int8_t value) {
    static uint8_t top = 0; //0 address is NULL (used for temporary storage for constants)

    Instruction instruction;
    instruction.arg2 = 0;
    if(top > NUM_VARIABLES - 1) {
        //Full stack
        return false;
    }
    top++;
    stack[top] = variable;

    instruction.opcode = INSTRUCTION_SET;
    instruction.arg1 = value;
    output_write_instruction(instruction);
    return top;
}


uint8_t variable_manager_get(char *name) {


    for(size_t i = 1; i < NUM_VARIABLES + 1; i++) {
        Variable var = stack[i - 1];
        if(strlen(name) == strlen(var.name)) {
            if(strcmp(name, var.name) == 0) {
                return i; //i is the address
            }
        }
    }

    return 0; //Not found
}



