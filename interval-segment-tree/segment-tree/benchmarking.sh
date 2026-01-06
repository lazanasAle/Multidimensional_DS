#! /bin/sh

make

for (( i=50000; i<=900000; i+=50000 ))
do
        ./segment_tree_test $i 6 9 2 10 >> /tmp/tmp_txt.txt
done

make clean
