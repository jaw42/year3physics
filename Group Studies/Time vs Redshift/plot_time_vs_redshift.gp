reset
set xrange[0:10]
#set yrange[0:1]

set title ""
set xlabel "Redshift"
set ylabel '\begin{sideways}Time (Gyr After the Big Bang)\end{sideways}'
set ytics mirror
set xtics mirror
set mxtics 10
set grid
set mytics 10
set term latex
set output "time_vs_redshift.tex"
unset key

unset logscale y

f(x) = 28 / ( 1 + ( 1+x )**2 )
g(x) = sqrt( (28/x) -1 ) - 1

plot g(x)

pause -1 "Hit return to continue"

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=time_vs_redshift.pdf'

# plot again to file:
replot