reset
set xrange[-4:4]
#set yrange[0:3]

set title ""
set xlabel "x / k"
set ylabel "y"
set samples 1000

#plot 1/(sqrt(0.2)*sqrt(2*pi))*exp(-((x-0)**2)/(2*0.2))
a=15
plot exp(-a*x**2) title "Gaussian" lw 5, sqrt(pi/a)*exp(-(pi**2*x**2)/a) title "Fourier Transform of Gaussian" lw 5

pause -1 "Hit return to continue"

# color plot
set term post enhanced color solid "Helvetica" 16
# bw plot
#set term post eps enhanced mono dashed "Helvetica" 18

# automatically create pdf file
set output '| epstopdf --filter --outfile=Report/gaussian.pdf'

# plot again to file:
replot
