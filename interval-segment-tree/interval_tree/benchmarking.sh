#! /bin/sh

make

for (( i=1000; i<=20000; i+=1000 ))
do
        ./interval_tree_test $i >> /tmp/tmp_txt.txt
done

make clean
