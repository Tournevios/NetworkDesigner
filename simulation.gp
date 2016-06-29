set title "Activity variation of the network"
set xlabel "Temperature"
set ylabel "Percentage %"
set yrange [0:101]
plot "simulation.dat" title 'Change to one rate' with lines
