#! /bin/bash

upper_bound_segment=$((50 + RANDOM % 500))

#make the files
make

min_dataset=1000
max_dataset=20000

for (( i=min_dataset; i<=max_dataset; i+=min_dataset ))
do
        #crete the dataset in python
        echo "Starting Experiment with $i data size"
        python SytheticData.py "$i" "$upper_bound_segment"
        ./SweepLine >> /tmp/tmp_txt.txt
        echo "Experiment $e finished."
        echo "--------------------------"
done

make clean
