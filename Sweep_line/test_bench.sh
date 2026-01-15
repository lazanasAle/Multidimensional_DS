#! /bin/bash

#choose the python interpreter

NOGIL_PYTHON="$1"

if [ -n "$NOGIL_PYTHON" ] && [ -x "$NOGIL_PYTHON" ]; then
        PYTHON="$NOGIL_PYTHON"
else
        PYTHON="$(command -v python3 || command -v python)"
fi

if [ -z "$PYTHON" ]; then
        echo "error No python interpreter found"
        exit 1
fi


upper_bound_segment=$((50 + RANDOM % 500))

#make the files

make

min_dataset=5000
max_dataset=100000

for (( i=min_dataset; i<=max_dataset; i+=min_dataset ))
do
        #crete the dataset in python
        echo "Starting Experiment with $i data size"
        PYTHON_GIL=0 "$PYTHON" SytheticData.py "$i" "$upper_bound_segment"
        ./SweepLine >> /tmp/tmp_txt.txt
        echo "Experiment $e finished."
        echo "--------------------------"
done

make clean
