#!/usr/cisco/bin/gnuplot

set terminal png size 2048,1024
set output "p_riter.png"

set autoscale
set xtic auto
set ytic auto
set grid
plot "p_riter.dat" with linespoints
