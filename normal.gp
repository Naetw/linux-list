reset
set term png enhanced font 'Verdana,10'
set ylabel "time (sec)"
set xlabel "size"
set xtics 10000
set title "Time Comparison (random input)"
set output "normal.png"

plot [:][-1:]'normal.txt' \
    using 1:2 with linespoints linewidth 2 title 'merge', \
''  using 1:3 with linespoints linewidth 2 title 'quick', \
''  using 1:4 with linespoints linewidth 2 title 'insert'

set output "qm.png"

plot 'normal.txt' \
    using 1:2 with linespoints linewidth 2 title 'merge', \
''  using 1:3 with linespoints linewidth 2 title 'quick', \
