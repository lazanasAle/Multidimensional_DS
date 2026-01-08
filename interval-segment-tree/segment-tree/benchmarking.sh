#! /bin/sh

make

max_idx=900000

for (( i=50000; i<=max_idx; i+=50000 ))
do
        adding_idx=$((RANDOM % max_idx))
        adding_key=$((RANDOM % 50))
        start_idx=$((RANDOM % max_idx))
        end_idx=$((start_idx + RANDOM % (max_idx - start_idx)))
        ./segment_tree_test $i $adding_idx $adding_key $start_idx $end_idx >> /tmp/tmp_txt.txt
done

make clean
