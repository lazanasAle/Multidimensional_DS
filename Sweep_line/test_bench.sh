#! /bin/bash

#choose the python interpreter

NOGIL_PYTHON="$1"
NOGIL_PRESENT=0

if [ -n "$NOGIL_PYTHON" ] && [ -x "$NOGIL_PYTHON" ]; then
        PYTHON="$NOGIL_PYTHON"
        NOGIL_PRESENT=1
else
        PYTHON="$(command -v python3 || command -v python)"
        NOGIL_PRESENT=0
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
        if [ "$NOGIL_PRESENT" -ne 0 ]; then
                PYTHON_GIL=0 "$PYTHON" SytheticData.py "$i" "$upper_bound_segment"
        else
                "$PYTHON" SytheticData.py "$i" "$upper_bound_segment"
        fi
        ./SweepLine >> /tmp/tmp_txt.txt
        echo "Experiment $i finished."
        echo "--------------------------"
done

make clean
