#include "../src/Tokenise.h"
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Usage: [./exec] [src.txt] [dest.txt]");
        return -1;
    }
    FILE *src = fopen(argv[1], "r");
    FILE *dest = fopen(argv[2], "w");
    if(!src || !dest) {
        if(src) fclose(src);
        if(dest) fclose(dest);
        printf("File read error");
        return -2;
    }


    fclose(src);
    fclose(dest);
    
    return 0;
}

