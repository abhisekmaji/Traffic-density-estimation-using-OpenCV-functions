#!/bin/sh
echo "Looping over method 1"
g++ -o m1 m1.cpp -std=c++11 `pkg-config --cflags --libs opencv`
for i in 1 2 3 4 5 6 7 8 9 10 11 12
do
  echo "i = $i"  
    ./m1 trafficvideo $i
done