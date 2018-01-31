#!/bin/bash
for i in {1..10};
do ./../cmake-build-debug/bigrammi "../files/inputs/input3.txt" >> results.txt;
echo $i;
done

