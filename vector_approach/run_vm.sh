#!/bin/bash
curr_dir=$PWD

#java -jar dataextraction.jar $curr_dir//data

g++-4.9.1 windrose_vector.cpp -o windrose_vector -fopenmp -std=gnu++11 
if [ $? -ne 0 ]
then
    echo "Compile failed!"
    exit 1
fi


#find files in currdir with .csv extension recursively
file_list=""
for entry in $(find $curr_dir -type f -name "*.csv")
do
	file_list="$file_list $entry"
done
export OMP_NUM_THREADS=4
time ./windrose_vector $file_list > run_log.txt