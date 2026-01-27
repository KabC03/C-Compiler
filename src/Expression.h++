#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <stdbool.h>
#include <string>
#include "Structures.h++"
#include "Tokenise.h++"
#include "Variable_Manager.h++"
#include "Output.h++"

uint16_t expression_evaluate(char **text, uint8_t destinationAddress);


#endif
