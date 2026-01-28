#include "Variable_Manager.h++"

std::vector<std::string> stack;

uint16_t variable_manager_add(std::string variable) {

    for(std::string var : stack) {
        if(var == variable) {
            return 0; //Duplicate variable
        }
    }

    stack.push_back(variable);
    return stack.size();
}


uint16_t variable_manager_get(char *name) {


    for(size_t i = 1; i < stack.size() + 1; i++) {
        
        std::string var = stack[i - 1];
        if(var == name) {
            return i; //Address
        }
    }

    return 0; //Not found
}



