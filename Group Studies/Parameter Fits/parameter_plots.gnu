reset
#set xrange[0:1]
#set yrange[0:1]

set title ""
set xlabel "Redshift"
set ylabel "Alpha"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
set key

f(x) = m * x + c
fit f(x) "alpha_fit.txt" using 1:3 via m, c
titlef = sprintf("%.2fx+%.2f", m, c)
g(x) = n*log(x) + d
fit g(x) "alpha_fit.txt" using 1:3 via n, d
titleg = sprintf("%.2flog(x)+%.2f", n, d)

plot\
"alpha_fit.txt" using 1:3:($3+$4):($3-$5) with yerrorbars lt 7, f(x) t titlef, g(x) t titleg

pause -1 "Hit return to continue"

# color plot
#set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
#set output '| epstopdf --filter --outfile=plot2D.pdf'

# plot again to file:
#replot