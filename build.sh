#!/usr/bin/env bash
rm -f ./freq
g++ freq.cpp -std=c++17 -O3 -o freq
echo "try your fresh build ./freq"
