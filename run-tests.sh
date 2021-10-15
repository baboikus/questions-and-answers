#!/usr/bin/env bash

RED=' \033[1;31m '
GREEN=' \033[1;32m '
DEFAULT=' \033[0m: '

rm -f ./freq
g++ freq.cpp -std=c++17 -o freq


echo 'doc example check...'
rm -f ./test_data/actual_out1.txt
./freq ./test_data/in1.txt ./test_data/actual_out1.txt
if diff ./test_data/actual_out1.txt ./test_data/expected_out1.txt; then
    echo "${GREEN}pass${DEFAULT}"
else
    echo "${RED}fail${DEFAULT}"
fi


echo 'custom example check...'
rm -f ./test_data/actual_out2.txt
./freq ./test_data/in2.txt ./test_data/actual_out2.txt
if diff ./test_data/actual_out2.txt ./test_data/expected_out2.txt; then
    echo "${GREEN}pass${DEFAULT}"
else
    echo "${RED}fail${DEFAULT}"
fi


echo 'not enought args check...'
./freq ./test_data/in2.txt
if [ $? -eq 1 ]; then
    echo "${GREEN}pass${DEFAULT}"
else
    echo "${RED}fail${DEFAULT}"
fi


echo 'too many args check...'
./freq ./test_data/in1.txt ./test_data/actual_out1.txt ./test_data/actual_out2.txt
if [ $? -eq 2 ]; then
    echo "${GREEN}pass${DEFAULT}"
else
    echo "${RED}fail${DEFAULT}"
fi


echo 'non existed input file check...'
rm -f ./test_data/non_existed_in.txt
rm -f ./test_data/non_existed_output.txt
./freq ./test_data/non_existed_in.txt ./test_data/non_existed_output.txt
if [ $? -eq 3 ]; then
    echo "${GREEN}pass${DEFAULT}"
else
    echo "${RED}fail${DEFAULT}"
fi


echo 'already existed output file check check...'
rm -f ./test_data/existed_output.txt
touch ./test_data/existed_output.txt
./freq ./test_data/in1.txt ./test_data/existed_output.txt
if [ $? -eq 4 ]; then
    echo "${GREEN} pass ${DEFAULT}"
else
    echo "${RED} fail ${DEFAULT}"
fi
