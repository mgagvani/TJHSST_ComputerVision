#!/bin/sh
echo "--- Compiling File ---"
g++ -o l02 ../Project1/l01lib.cpp l02lib.cpp l02.cpp -Wall -Wpedantic -Wextra -Ofast
echo "--- Running Code ---"
./l02
echo "--- Converting to JPG ---"
convert l02.ppm l02.jpg
cd Images
convert out1.ppm out1.jpg
convert out2.ppm out2.jpg
convert out3.ppm out3.jpg
convert out4.ppm out4.jpg
convert out5.ppm out5.jpg
convert out6.ppm out6.jpg
cd ..
rm l02