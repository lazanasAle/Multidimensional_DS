import csv
import random
from dataclasses import fields

Size_of_dataset=20000
Upper_bound=200 #the upper bound for thr interval


#create data set for interval & segment trees & swipe line
intervals = [
    (int(random.uniform(0, Upper_bound)), int(random.uniform(0, Upper_bound)))
    for i in range(Size_of_dataset)
]

fields= ['X','Y']

with open('Interval_Segmnet.csv','w',newline='') as f:
    writer=csv.writer(f)
    writer.writerow(fields)
    writer.writerows(intervals)

