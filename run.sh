#!/bin/bash

g++-5 windrose.cpp -o windrose -fopenmp -std=gnu++11 
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
export OMP_NUM_THREADS=1
time ./windrose $file_list