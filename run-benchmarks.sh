#!/usr/bin/env bash

rm -f ./freq
g++ freq.cpp -std=c++17 -O3 -pg -o freq

echo 'lets process some huge file'
rm -f ./test_data/huge_out.txt
time ./freq ./test_data/huge.txt ./test_data/huge_out.txt
rm -f ./gprof.txt
gprof freq gmon.out >> gprof.txt
