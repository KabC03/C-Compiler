#include "../src/Vector.h"
#include "../src/Hashmap.h"
#include "../src/Tokenise.h"

int main(void) {
    tokenise_generate_token_set();   
    char str[] = "hello";

    tokenise_get_next_token(str);

 
    tokenise_destroy_token_set();
    return 0;
}

