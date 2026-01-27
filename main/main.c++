#include "../src/Tokenise.h++"
#include "../src/Output.h++"
#include "../src/Parse.h++"
#include <time.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if(argc != 3) {
        printf("Usage: [./exec] [src.txt] [dest.txt]");
        return -1;
    }
    if(!output_set_file(argv[1])) {
        printf("File read error for output\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[2], "r");
    if(!inputFile) {
        printf("File read error for input\n");
        return -1;
    }


    long cur = ftell(inputFile);
    fseek(inputFile, 0, SEEK_END);
    long size = ftell(inputFile);
    fseek(inputFile, cur, SEEK_SET);


    char *buffer = (char*)malloc(size);
    if(!buffer) {
        printf("Input file alloc error\n");
        return -1;
    } else {
        fread(buffer, 1, size, inputFile);
    }

    auto lines = parse_parse(&buffer);
    printf("Output is %hu lines\n", lines);
    
    fclose(inputFile);
    output_close_file();
    
    return 0;
}

