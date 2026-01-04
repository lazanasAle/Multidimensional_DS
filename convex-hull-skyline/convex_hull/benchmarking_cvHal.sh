#! /bin/sh

make

for (( i=50000; i<=950000; i+=50000 ))
do
        ./ConvexHull $i >> /tmp/tmp_txt.txt
done

make clean
