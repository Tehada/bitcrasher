#!/bin/bash
for i in {1..11}
do
    cpaldjvu -colors 2 $i.pgm $i.djvu
    ddjvu -format=pbm $i.djvu $i.pbm
done