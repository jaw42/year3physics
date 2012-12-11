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
set zrange[-1:1]

set title 
set xlabel "x"
set ylabel "Electric and Magnetic Feilds (Arbitary Units)"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
unset key
#set term x11

#splot "/tmp/fdtd/data100.txt" using 1:2:4 w l title "Electric Field" lw 5, "/tmp/fdtd/data100.txt" using 1:4:3 w l title "Magnetic Field" lw 5

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=Report/THISISATEST.pdf'

# plot again to file:
replot

pause -1 "Continue?"
