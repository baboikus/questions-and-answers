#!/usr/bin/env bash

RED=' \033[1;31m '
GREEN=' \033[1;32m '
DEFAULT=' \033[0m '

rm -f ./freq
g++ freq.cpp -std=c++17 -O3 -o freq


echo 'doc example check...'
rm -f ./test_data/actual_out1.txt
./freq ./test_data/in1.txt ./test_data/actual_out1.txt
if diff ./test_data/actual_out1.txt ./test_data/expected_out1.txt; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi


echo 'custom example check...'
rm -f ./test_data/actual_out2.txt
./freq ./test_data/in2.txt ./test_data/actual_out2.txt
if diff ./test_data/actual_out2.txt ./test_data/expected_out2.txt; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi

echo 'sample huge sorting crash check...'
rm -f ./test_data/sample_huge_out.txt
./freq ./test_data/sample_huge_in.txt ./test_data/sample_huge_out.txt
if [ $? -eq 0 ]; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi

echo 'not enought args check...'
./freq ./test_data/in2.txt
if [ $? -eq 1 ]; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi


echo 'too many args check...'
./freq ./test_data/in1.txt ./test_data/actual_out1.txt ./test_data/actual_out2.txt
if [ $? -eq 2 ]; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi


echo 'non existed input file check...'
rm -f ./test_data/non_existed_in.txt
rm -f ./test_data/non_existed_output.txt
./freq ./test_data/non_existed_in.txt ./test_data/non_existed_output.txt
if [ $? -eq 3 ]; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi


echo 'already existed output file check check...'
rm -f ./test_data/existed_output.txt
touch ./test_data/existed_output.txt
./freq ./test_data/in1.txt ./test_data/existed_output.txt
if [ $? -eq 4 ]; then
    printf "${GREEN}pass${DEFAULT}\n"
else
    printf "${RED}fail${DEFAULT}\n"
fi
