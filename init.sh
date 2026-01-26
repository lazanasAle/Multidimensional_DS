#! /bin/sh

cd convex-hull-skyline
unzip data_movies_clean.zip
cd ..

cd 3d-rtree\(trajectories\)
unzip tartanaviation_adsb_900k.zip

python generate-readable.py
cd ..

make
