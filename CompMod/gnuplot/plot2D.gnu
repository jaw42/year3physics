#-----------------------------------------------------------------
# Gnuplot example file plot2D
# Andreas Freise 17.09.2011
#
# Simple example file for plotting a simple 2D plot from the
# data provided in the file 'test1.dat'.
#
#-----------------------------------------------------------------

reset
set xrange[0:1]
set yrange[0:1]

set title ""
set xlabel "x"
set ylabel "y"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
set size ratio 1
unset key
set term x11

plot\
"test1.dat" using ($1):($2) w l

pause -1 "Hit return to continue"

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=plot2D.pdf'

# plot again to file:
replot