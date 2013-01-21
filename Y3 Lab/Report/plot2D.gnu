reset
#set xrange[0:1600]
#set yrange[0:0.3]

set title ""
set xlabel "Energy (keV)"
set ylabel "Efficiency ({/Symbol a})"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
unset key
set term x11
#set term dumb

#f(x) = c * x**(-m)
f(x) = exp(-m*x + c)
c=10
m=10
fit f(x) "data.dat" using (($1)):(($8)) via m,c

plot "data.dat" using (($1)):(($8)) title "BF3 Data", f(x) lc 000000

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=BF3countsvsradius.pdf'

# plot again to file:
replot
