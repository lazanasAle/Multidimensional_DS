#! /bin/bash

upper_bound_segment=$((50 + RANDOM % 500))
echo -n "Enter the size of the dataset: "
read dataset

while [[ "$dataset" -le 9000 ]];
do
        echo -n "The saize of the dataset must be > 9000. Please enter again:"
        read dataset
done

#make the files
make

echo -n "Enter number of experments:"
read experments

for e in "${experiments}";
do
        #crete the dataset in python
        echo "Starting Experiment $((e+1)) of $experiments"
        python SytheticData.py "$dataset" "$upper_bound_segment"
        ./SweepLine
        echo "Experiment $e finished."
        echo "--------------------------"
done

make clean
