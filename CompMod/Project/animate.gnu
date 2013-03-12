#-----------------------------------------------------------------
# Gnuplot example file plot2D
# Andreas Freise 17.09.2011
#
# Simple example file for plotting a simple 2D plot from the
# data provided in the file 'test1.dat'.
#
#-----------------------------------------------------------------

reset

#set terminal gif animate delay 10
set terminal gif 
#set output "Animation.gif"
set output '| display gif:-'

set xrange[0:200]
set yrange[0:1]

set title "Animation"
set xlabel "x"
set ylabel "Electric Field"
set ytics mirror
set xtics mirror

plot "data10.txt"
plot "data20.txt"
plot "data30.txt"
plot "data40.txt"
plot "data50.txt"
plot "data60.txt"
plot "data70.txt"
plot "data80.txt"
plot "data90.txt"
plot "data100.txt"
plot "data110.txt"
plot "data120.txt"
plot "data130.txt"
plot "data140.txt"
plot "data150.txt"
plot "data160.txt"
plot "data170.txt"
plot "data180.txt"
plot "data190.txt"


