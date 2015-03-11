#!/bin/bash

if [ z"$1" != "z" ]; then
    pgm="$1"
else
    pgm="poly.c"
fi

echo COMPILING: ${pgm}

gcc -Wall ${pgm} -lm
./a.out > p.dat
cat p.dat | grep A: | cut -f2- -d: > p_orig.dat
cat p.dat | grep B: | cut -f2- -d: > p_iter.dat
cat p.dat | grep C: | cut -f2- -d: > p_riter.dat
cat p.dat | grep D: | cut -f2- -d: > p_riter2.dat
./plt.sh
./plt1.sh

