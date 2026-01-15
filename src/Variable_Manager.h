#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H
#include <stdbool.h>
#include <string.h>
#include "Structures.h"
#include "Output.h"


uint8_t variable_manager_add(Variable variable, int8_t value);
uint8_t variable_manager_get(char *name);



#endif
