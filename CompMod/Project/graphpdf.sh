#! /bin/bash

#name=$(zenity --entry --text "Enter a name for the file:")
#number=$(zenity --entry --text "Enterthe frame number:")
read -p "Enter the name of the file: " -e name

while :
do 
	read -p "Enter the frame number: " -e number
	echo "reset 
	set xrange[0:200] 
	# set yrange[-0.4:0.4]
	set yrange[-1:1]
	set zrange[-1:1]

	set title ""
	set xlabel \"x\"
	set ylabel \"Electric and Magnetic Feilds (Arbitary Units)\"
	set ytics mirror
	set xtics mirror
	set mxtics 10
	set grid
	set mytics 10
	unset key
	#set term x11

	# set object 1 rect from 20,-1 to 200,1 fc rgb \"#F0F3F5\"
	plot \"/tmp/fdtd/data$number.txt\" using 1:2 w l title \"Electric Field\" lw 2, \"/tmp/fdtd/data$number.txt\" using 1:3 w l title \"Magnetic Field\" lw 2

	# color plot
	set term post enhanced color solid \"Helvetica\" 16
	# bw plot
	#set term post eps enhanced mono dashed \"Helvetica\" 18

	# automatically create pdf file
	set output '| epstopdf --filter --outfile=Report/$name.pdf'

	# plot again to file:
	replot

	pause -1 \"Continue?\"" > pdfs.gnu

	gnuplot pdfs.gnu 
done
