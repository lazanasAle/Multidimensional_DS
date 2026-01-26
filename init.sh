#! /bin/sh


unzip convex-hull-skyline/data_movies_clean.zip

unzip 3d-rtree\(trajectories\)/tartanaviation_adsb_900k.zip

python generate-readable.py

make
