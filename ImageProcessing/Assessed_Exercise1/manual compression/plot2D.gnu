reset
#set xrange[0:1600]
set yrange[0:1]

set title ""
set xlabel "Percentage Removed"
set ylabel "File Size (Normalised)"
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
unset key
set term x11
#set term dumb

#f(x) = c * x**(-m)
#f(x) = exp(m*x + c)
#c=10
#m=10
#fit f(x) "filesizes.txt" via m,c

plot "filesizes_normalised_all.txt" using (100-$2):3 w l lw 5 lc 000000, "filesizes_normalised_all.txt" using (100-$2):3 ps 1.5 lw 5, "filesizes_normalised_all.txt" using (100-$2):4 w l lw 5 lc 000000, "filesizes_normalised_all.txt" using (100-$2):4 ps 1.5 lw 5

#plot "filesizes.txt" w l lw 5, "filesizes.txt" ps 1.5 lw 5 lc 000000

pause -1 "Hit return to continue"

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=comparason_graph.pdf'

# plot again to file:
replot
