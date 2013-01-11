reset
#set xrange[0:1]
#set yrange[0:1]

set title ""
set xlabel "Channel Number"
set ylabel "ln(Number of Counts)"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
unset key
set term x11

#f(x) = m*x + c
#fit f(x) "bf3data.txt" using (($1)):(log($2)) via m,c

plot "bf3data.txt" using (($1)):(log($2)*($1)) title "BF3 Data"
#, f(x)

pause -1 "Hit return to continue"

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=BF3data1.pdf'

# plot again to file:
replot
