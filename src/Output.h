#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include <stdbool.h>
#include "Instructions.h"
#include "Structures.h"

bool output_set_file(char *fileName);
void output_write_instruction(Instruction instruction);
void output_close_file(void);

#endif
