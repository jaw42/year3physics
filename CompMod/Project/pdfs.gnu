reset 
	set xrange[0:200] 
	# set yrange[-0.4:0.4]
	set yrange[-1:1]
	set zrange[-1:1]

	set title 
	set xlabel "z"
	set ylabel "Electric and Magnetic Feilds (Arbitary Units)"
	set ytics mirror
	set xtics mirror
	set mxtics 10
#	set grid
	set mytics 10
	unset key
#	set term x11


	set object 1 rect from 100,-1 to 200,1 fc rgb "#F0F3F5"
	plot "/tmp/fdtd/data50.txt" using 1:2 w l title "Electric Field" lw 2 lc rgb "#BFBFBF", "/tmp/fdtd/data50.txt" using 1:3 w l title "Magnetic Field" lw 1 lc rgb "#BFBFBF", \
		 "/tmp/fdtd/data250.txt" using 1:2 w l title "Electric Field" lw 2 lc rgb "red", "/tmp/fdtd/data250.txt" using 1:3 w l title "Magnetic Field" lw 2 lc rgb "green", \
		 -0.5 lw 2 lc rgb "#BcBcBc", \
		 0.617 lw 2 lc rgb "#B0B0B0"

	# color plot
	set term post enhanced color solid "Helvetica" 16
	# bw plot
	#set term post eps enhanced mono dashed "Helvetica" 18

	# automatically create pdf file
	set output '| epstopdf --filter --outfile=Report/NEXT.pdf'

	# plot again to file:
	replot

	pause -1 "Continue?"
