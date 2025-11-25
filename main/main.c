#include "../src/Vector.h"
#include "../src/Hashmap.h"
#include "../src/Tokenise.h"

int main(void) {
    tokenise_generate_token_set();   

    char *str = "abc= +++";
    Token token = tokenise_consume(&str);
    printf("%d, '%s'\n", token.id, token.str);
    token = tokenise_consume(&str);
    printf("%d, '%s'\n", token.id, token.str);

    tokenise_destroy_token_set();
    return 0;
}

