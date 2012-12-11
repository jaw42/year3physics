#-----------------------------------------------------------------
# Gnuplot example file plot2D
# Andreas Freise 17.09.2011
#
# Simple example file for plotting a simple 2D plot from the
# data provided in the file 'test1.dat'.
#
#-----------------------------------------------------------------

reset 
set xrange[0:200] 
set yrange[-1:1]

set title ""
set xlabel "x"
set ylabel "Electric Feild"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
unset key
set term x11

#filename(n) = sprintf("data%d", n)
#plot for [i=1:2000:10] filename(i) using 1:2 with lines
plot "data0400.txt" w l
plot "data0120.txt" w l
plot "data1500.txt" w l

pause -1 "Continue?"

