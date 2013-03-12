reset
set terminal latex
set output "inverse_FT_filter.tex"

set xrange[-10:10]

set title ""
set xlabel ""
set ylabel ""
unset key
set samples 100000

f(x) = (2*sin(pi*x))/(pi*x)-((sin(pi*x/2))/(pi*x/2))**2

plot f(x),  "filter_values.txt" lt 7

pause -1 "Hit return to continue"
