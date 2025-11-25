clear
clear
clear

if [[ $# -eq 0 ]]; then
    echo "No arguments provided for $0"
    exit -1
fi


mkdir -p ./bin

if [[ "$1" == "build" ]]; then
    clang ./src/*.c ./main/*.c -o ./bin/out -O3 -Wall -Wextra -Werror -Wpedantic -lm
elif [[ "$1" == "testing" ]]; then
    clang ./src/*.c ./main/*.c -o ./bin/out -O0 -fsanitize=address -Wall -Wextra -Werror -Wpedantic -lm
else
    printf "\n\n[SHELL] Unknown argument: %s\n\n" "$1"
    exit -1
fi


if [[ $? -ne 0 ]]; then
    printf "\n\n[SHELL] Unable to compile code\n\n"
    exit -2
else
    printf "\n\n[SHELL] Compilation success\n\n"
fi

./bin/out "../src.txt" "../dest.txt"
if [[ $? -ne 0 ]]; then
    printf "\n\n[SHELL] Runtime error\n\n"
    exit -3
else
    printf "\n\n[SHELL] Successful run\n\n"
fi





