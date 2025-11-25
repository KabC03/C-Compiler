#include "../src/Vector.h"
#include "../src/Hashmap.h"
#include "../src/Tokenise.h"
#include <time.h>

int main(void) {

    char *str = "WHILE IF FOR, + - +++ == IF DEF WHILE WHILE WHILE";
    Token token = {TOKEN_IDENTIFIER, "\0"};
    clock_t start = clock();
    while(token.id != TOKEN_INVALID) {
        token = tokenise_consume(&str);
        printf("%s\n", token.str);
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time: %f", time);

    return 0;
}

