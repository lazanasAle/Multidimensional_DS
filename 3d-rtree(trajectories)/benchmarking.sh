#! /bin/sh


make


min_data_size=50000
max_data_size=900000

for (( i=min_data_size; i<=max_data_size; i+=min_data_size ))
do
        ./test_rtree $i >> /tmp/tmp_txt.txt
done

make clean
