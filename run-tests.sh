#!/usr/bin/env bash

rm -f ./freq
g++ freq.cpp -std=c++17 -o freq

echo 'example check...'
rm -f ./test_data/actual_out1.txt
./freq ./test_data/in1.txt ./test_data/actual_out1.txt
diff ./test_data/actual_out1.txt ./test_data/expected_out1.txt
