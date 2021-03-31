#!/bin/sh
echo "Looping over method 2"
g++ -o m2 m2.cpp -std=c++11 `pkg-config --cflags --libs opencv`
for i in 1 2 3 4 5 6 7 8 9 10 11 12
do
  echo "i = $i"  
    ./m2 trafficvideo $i
done