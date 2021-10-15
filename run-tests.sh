#!/usr/bin/env bash

g++ freq.cpp -o freq

./freq ./test_data/in1.txt ./test_data/actual_out1.txt
diff ./test_data/actual_out1.txt ./test_data/expected_out1.txt
