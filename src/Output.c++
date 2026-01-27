#include "Output.h++"

FILE *file = NULL;

bool output_set_file(char *fileName) {
    file = fopen(fileName, "w");
    if(!file) {
        return false;
    }

    return true;
}

void output_write_instruction(Instruction instruction) {

    if(instruction.arg3[0] == '\0') {
        fprintf(file, "%s %d %d\n", INSTRUCTION_IMPLEMENTATION[instruction.opcode], instruction.arg1, instruction.arg2);
    } else {
        fprintf(file, "%s %d %d %s\n", INSTRUCTION_IMPLEMENTATION[instruction.opcode], instruction.arg1, instruction.arg2, instruction.arg3.c_str());
    }


    //Note that instructions may contain garbage, intepreter should ignore garbage values
    return;
}

void output_close_file(void) {
    fclose(file);
    return;
}


