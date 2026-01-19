#include "Variable_Manager.h"

Variable *stack; //Stack memory (VECTORISE THIS)
uint8_t stackSize = 0;
uint8_t stackTop = 0;

uint8_t variable_manager_add(Variable variable, int8_t value) {

    Instruction instruction;
    instruction.arg2 = 0;
    instruction.str[0] = '\0';
    if(stackSize > stackTop) {
        //Full stack
        stack = realloc(stack, stackTop * 2);
        stackTop *= 2;
        if(!stack) {
            return 0;
        }
    }
    stackSize++;
    stack[stackSize] = variable;

    instruction.opcode = INSTRUCTION_SET;
    instruction.arg1 = value;
    output_write_instruction(instruction);
    return stackSize;
}


uint8_t variable_manager_get(char *name) {


    for(size_t i = 1; i < stackSize + 1; i++) {
        Variable var = stack[i - 1];
        if(strlen(name) == strlen(var.name)) {
            if(strcmp(name, var.name) == 0) {
                return i; //i is the address
            }
        }
    }

    return 0; //Not found
}

void variable_manager_destory(void) {
    free(stack);
    return;
}


