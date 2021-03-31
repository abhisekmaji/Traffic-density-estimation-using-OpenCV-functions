#!/bin/sh
echo "Looping over method 3"
g++ -o m3 m3.cpp -std=c++11 -lpthread `pkg-config --cflags --libs opencv`
g++ -o error error.cpp
for i in 1 2 4 8 12 16 20 24 28 32
do
  echo "-----------------------------------------"
  echo "Number of threads = $i"
    ./m3 trafficvideo $i
    ./error 3
done
