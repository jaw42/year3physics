#-----------------------------------------------------------------
# Gnuplot example file plot3D
# Andreas Freise 17.09.2011
#
# Simple example file for plotting a 3D plot from the 
# data provided in the file `test2.dat'.
#
#-----------------------------------------------------------------

reset
set xlabel "f [MHz]"
set label "PZT feedback [Hz/{/Symbol \326}Hz]" at screen .05,.3 rotate
set mxtics 2
set mytics 2
set zero 0.0

#set format z '%.1g'
#set format y '%.1g'

set title ""
set size ratio 0.6
unset grid

#set pm3d at s hidden3d 100 solid
set pm3d at s 
#set pm3d map
#set view 66, 212, ,
#set cbrange[-200:200]
#set cntrparam levels 20
#set contour
#set view 0, 0, ,
set nosurface
unset hidden3d
set colorbox vertical
#set colorbox user origin .95,.1 size .04,.8
#set colorbox noborder
#set style line 100 lt 19 lw 0 #gif1
set style line 100 lt -1 lw 0

#-----------------------Colors--------------------------------
# -- traditional pm3d (black-blue-red-yellow)
#set palette rgbformulae 7,5,15
# -- green-red-violet
#set palette rgbformulae 3,11,6
# -- ocean (green-blue-white)  try also all other permutations
set palette rgbformulae 23,28,3
# -- hot  (black-red-yellow-white)
#set palette rgbformulae 21,22,23
# --  colour printable on gray (black-blue-violet-yellow-white)
#set palette rgbformulae 30,31,32
# -- rainbow (blue-green-yellow-red)
#set palette rgbformulae 33,13,10
# -- AFM hot (black-red-yellow-white)
#set palette rgbformulae 34,35,36
#-------------------------------------------------------------


splot\
"test2.dat" using ($1):($2):($3) title "Donut Mode"  w l

pause -1 "Hit return to continue"

# no plot as pdf and png file
set size ratio 1
unset label
set ylabel "position [m]"
set cblabel "relative power [W]"
set term post eps enhanced color dashed dl 3.0 "Helvetica" 17
set output '| epstopdf --filter --outfile=plot3D.pdf'
replot

set term png transparent small xffffff x000000 x404040  x9500d3 
set output "plot3D.png"
replot
