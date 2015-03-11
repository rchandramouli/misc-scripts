#!/usr/cisco/bin/gnuplot

set terminal png size 3072,1024
set output "p_iter.png"

set multiplot layout 1,3 title "Random polygon and iterates (91-sides)"

set autoscale
set xtic auto
set ytic auto
set ztic auto
set grid
set title "Reverse Iteration (10000 counts)" font ",14"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"
splot "p_riter.dat" with linespoints, "p_riter2.dat" with linespoints

set autoscale
set xtic auto
set ytic auto
set ztic auto
set grid
set title "Random polygon sample" font ",14"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"
splot "p_orig.dat" with linespoints

set autoscale
set xtic auto
set ytic auto
set ztic auto
set grid
set title "Forward Iteration (10000 counts)" font ",14"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"
splot "p_iter.dat" with linespoints
#, "p_iter_xy.dat" with lines, "p_iter_yz.dat" with lines, "p_iter_xz.dat" with lines

