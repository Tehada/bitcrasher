#!/bin/bash
for i in {1..11}
do
    montage -geometry 500 original/$i.pgm djvulibre/$i.pbm simpledjvu_norm_40_iters/$i.pbm bitcrasher/$i.pbm compare/$i.jpg
done