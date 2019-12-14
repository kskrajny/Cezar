#!/bin/bash
g++ main.cpp -o main
for i in *.in;
do
     echo $i
    ./main < $i > out1
    ./brut < $i > out2
    diff out1 out2
done
echo "po for";
for i in 1 2 3;
do
    echo $i
    ./gen 50000 500 > testgen
    ./main < testgen > out1
    echo "main finished"
    ./brut < testgen > out2
    diff out1 out2
done
