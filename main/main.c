#include "../src/Tokenise.h"
#include "../src/Output.h"
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Usage: [./exec] [src.txt] [dest.txt]");
        return -1;
    }
    FILE *dest = fopen(argv[2], "w");
    if(!output_set_file(argv[1])) {
        printf("File read error\n");
        return 1;
    }


    output_close_file();
    
    return 0;
}

