set xlabel "N Threads" font "Helvetica, 12"
set ylabel "SpeedUp" font "Helvetica, 12"
set title "SpeedUp QuickSort Hoare Pivot" font "Helvetica, 15"
set xrange [0:16]
set yrange [0:5]
set grid
set xtics 1
plot "quickh" using 1:2 title "QuickSort" with lines lc "red" lw 2