#!/bin/sh
echo "--- Compiling File ---"
g++ -o l02 ../Project1/l01lib.cpp l02.cpp -Wall -Wpedantic -Wextra -Ofast
echo "--- Running Code ---"
./l02
echo "--- Converting to JPG ---"
convert l02.ppm l02.jpg