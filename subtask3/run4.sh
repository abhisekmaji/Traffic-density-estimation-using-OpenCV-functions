#!/bin/sh
echo "Looping over method 4"
g++ -o m4 method4.cpp -std=c++11 -lpthread `pkg-config --cflags --libs opencv`
g++ -o error error.cpp
for i in 1 2 4 8 12 16 20 24 28 32
do
  echo "-----------------------------------------"
  echo "Number of threads = $i"  
    ./m4 trafficvideo $i
    ./error 4
done
