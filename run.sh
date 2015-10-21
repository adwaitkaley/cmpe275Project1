#!/bin/bash

g++-5 windrose.cpp -o windrose -fopenmp -std=gnu++11
if [ $? -ne 0 ]
then
    echo "Compile failed!"
    exit 1
fi
#time ./windrose /home/adminvm/Desktop/Data_meso/data/20020621_0800.csv /home/adminvm/Desktop/Data_meso/data/20030621_0800.csv /home/adminvm/Desktop/Data_meso/data/20040621_0800.csv /home/adminvm/Desktop/Data_meso/data/20050621_0800.csv /home/adminvm/Desktop/Data_meso/data/20060621_0800.csv /home/adminvm/Desktop/Data_meso/data/20070621_0800.csv /home/adminvm/Desktop/Data_meso/data/20080621_0800.csv /home/adminvm/Desktop/Data_meso/data/20090621_0800.csv /home/adminvm/Desktop/Data_meso/data/20100621_0800.csv /home/adminvm/Desktop/Data_meso/data/20110621_0800.csv /home/adminvm/Desktop/Data_meso/data/20120621_0800.csv /home/adminvm/Desktop/Data_meso/data/20130621_0800.csv /home/adminvm/Desktop/Data_meso/data/20140621_0800.csv /home/adminvm/Desktop/Data_meso/data/20020621_0800.csv /home/adminvm/Desktop/Data_meso/data/20030621_0800.csv /home/adminvm/Desktop/Data_meso/data/20040621_0800.csv /home/adminvm/Desktop/Data_meso/data/20050621_0800.csv /home/adminvm/Desktop/Data_meso/data/20060621_0800.csv /home/adminvm/Desktop/Data_meso/data/20070621_0800.csv /home/adminvm/Desktop/Data_meso/data/20080621_0800.csv /home/adminvm/Desktop/Data_meso/data/20090621_0800.csv /home/adminvm/Desktop/Data_meso/data/20100621_0800.csv /home/adminvm/Desktop/Data_meso/data/20110621_0800.csv /home/adminvm/Desktop/Data_meso/data/20120621_0800.csv /home/adminvm/Desktop/Data_meso/data/20130621_0800.csv /home/adminvm/Desktop/Data_meso/data/20140621_0800.csv   
curr_dir=$PWD
#find files in currdir with .csv extension recursively
file_list=""
for entry in $(find $curr_dir -type f -name "*.csv")
do
	file_list="$file_list $entry"
done
time ./windrose $file_list
