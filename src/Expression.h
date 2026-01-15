#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <stdbool.h>
#include "Structures.h"
#include "Tokenise.h"
#include "Variable_Manager.h"

bool expression_evaluate(char *text, uint8_t destinationAddress);


#endif
