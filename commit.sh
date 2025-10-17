git add .


if [[ $# -eq 0 ]]; then
    echo "No arguments provided for $0"
    exit -1
fi


if [[ $? -ne 0 ]]; then
    printf "\n\n[SHELL] Unable to add to staging area\n\n"
    exit -1
fi

git commit -m "$1"
if [[ $? -ne 0 ]]; then
    printf "\n\n[SHELL] Unable to commit to repo\n\n"
    exit -2
fi


git push --force
if [[ $? -ne 0 ]]; then
    printf "\n\n[SHELL] Unable to push to repo\n\n"
    exit -3
fi

sleep 1
clear
