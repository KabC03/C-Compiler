#ifndef VARIABLE_MANAGER_H
#define VARIABLE_MANAGER_H
#include <stdbool.h>
#include <vector>
#include <string.h>
#include <stack>
#include "Structures.h++"
#include "Output.h++"


uint16_t variable_manager_add(std::string variable, int8_t value);
uint8_t variable_manager_get(char *name);


#endif
