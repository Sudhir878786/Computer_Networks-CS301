set terminal png
set output "CW.png"
set title "Congestion window Plot"
set xlabel "Time (Seconds)"
set ylabel "Congestion Window"

plot "tcp-demo.cwnd" using 1:2 with linespoints title "Old Cwnd", "tcp-demo.cwnd" using 1:3 with linespoints title "New Cwnd"
