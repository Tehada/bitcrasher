#!/bin/bash
for i in {1..11}
do
    ./bitcrasher images/original/$i.pgm images/test_bitcrasher/$i.pbm
done