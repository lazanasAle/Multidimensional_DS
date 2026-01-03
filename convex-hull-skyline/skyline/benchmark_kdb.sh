#! /bin/sh

make

for (( i=50000; i<=900000; i+= 50000 ))
do
        ./kd_btree_test $i 2000 8000 1000 3000 2 6 3 7 2 4 1 16 min min min min min min >> /tmp/tmp_txt.txt
done

make clean
