#! /usr/local/cs/bin/gnuplot
#
# purpose:
#  generate data reduction graphs for the multi-threaded list project
#
# input: lab2b_list.csv
# 1. test name
# 2. # threads
# 3. # iterations per thread
# 4. # lists
# 5. # operations performed (threads x iterations x (ins + lookup + delete))
# 6. run time (ns)
# 7. run time per operation (ns)
#
# output:
# lab2b_1.png ... 
#

# general plot parameters
set terminal png
set datafile separator ","

# lab2b_1.png
# What is the throughput for different thread counts?
set title "List-1: Throughput vs Thread Number"
set xlabel "Threads"
set logscale x 10
set ylabel "Throughput (ops/s)"
set logscale y 10
set output 'lab2b_1.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title 'mutex' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title 'spin lock' with linespoints lc rgb 'red'     

# lab2b_2.png
# What is the throughput for different thread counts?
set title "List-2: Wait for lock time vs Thread Number"
set xlabel "Threads"
set logscale x 10
set ylabel "seconds"
set logscale y 10
set output 'lab2b_2.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title 'throughput' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
     title 'average wait for lock' with linespoints lc rgb 'red'

# lab2b_3.png
set title "List-3: Threads and Iterations that run without failure"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:]
set yrange [1:250]
set ylabel "Iterations per thread"
set logscale y 10
set output 'lab2b_3.png'
plot \
     "< grep list-id-s lab2b_list.csv" using ($2):($3) \
     title 'spin-lock w/yields=id' with points lc rgb 'blue', \
     "< grep list-id-m lab2b_list.csv" using ($2):($3) \
     title 'mutex w/yields=id' with points lc rgb 'green', \
     "< grep list-id-none lab2b_list.csv" using ($2):($3) \
     title 'unprotected w/yields=id' with points lc rgb 'red'

# lab2b_4.png
# Mutex throughput for different multi-list numbers
set title "List-4: Mutex Throughput vs Thread Number"
set xlabel "Threads"
set logscale x 10
set ylabel "Throughput (ops/s)"
set logscale y 10
set yrange [5000:8000000]
set output 'lab2b_4.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep -e 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '1 list' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '4 lists' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '8 lists' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '16 lists' with linespoints lc rgb 'orange'



# lab2b_5.png
# What is the throughput for different thread counts and list numbers?
# Spin-lock throughput for different multi-list numbers
set title "List-5: Spin-lock Throughput vs Thread Number"
set xlabel "Threads"
set logscale x 10
set ylabel "Throughput (ops/s)"
set logscale y 10
set yrange [5000:8000000]
set output 'lab2b_5.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep -e 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '1 list' with linespoints lc rgb 'green', \
     "< grep -e 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '4 lists' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '8 lists' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
     title '16 lists' with linespoints lc rgb 'orange'