#!/bin/bash

#g++-5 windrose.cpp -o windrose -fopenmp -std=gnu++11 

#for parallel run wind.cpp
export OMP_NUM_THREADS=4
g++-4.9 -g parallel.cpp -o windrose -fopenmp -std=gnu++11

#for serial
#export OMP_NUM_THREADS=1
#g++-4.9 -g working.cpp -o windrose -fopenmp -std=gnu++11
if [ $? -ne 0 ]
then
    echo "Compile failed!"
    exit 1
fi

curr_dir=$PWD
#find files in currdir with .csv extension recursively
file_list=""
for entry in $(find $curr_dir -type f -name "*.csv")
do
	file_list="$file_list $entry"
done

time ./windrose $file_list
#valgrind --tool=memcheck ./windrose $file_list
