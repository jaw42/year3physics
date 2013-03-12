reset
#set xrange[0:1600]
#set yrange[0:0.3]

set title ""
set xlabel "Quality Value"
set ylabel "File Size (bytes)"
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

plot "filesizes.txt" w l lw 5, "filesizes.txt" ps 1.5 lw 5 lc 000000

#pause -1 "Hit return to continue"

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=qualityvsfilesize.pdf'

# plot again to file:
replot
