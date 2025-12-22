import csv
import random
from dataclasses import fields

Size_of_dataset=20000
Upper_bound=200 #the upper bound for thr interval


#create data set for interval & segment trees & swipe line

intervals = []

for i in range (Size_of_dataset):
    a = int(random.uniform(0, Upper_bound))
    b = int(random.uniform(a, Upper_bound))
    intervals.append((a, b))

fields= ['X','Y']

with open('Interval_Data.csv','w',newline='') as f:
    writer=csv.writer(f)
    writer.writerow(fields)
    writer.writerows(intervals)

