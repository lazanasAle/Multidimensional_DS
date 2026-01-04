import csv
import random
from dataclasses import fields

Size_of_dataset = 20
Upper_bound = 200  # the upper bound for thr interval

lines = [
    (
        int(random.uniform(0, Upper_bound)),
        int(random.uniform(0, Upper_bound)),
        int(random.uniform(0, Upper_bound)),
        int(random.uniform(0, Upper_bound)),
        f"s{i}",
    )
    for i in range(Size_of_dataset)
]

fields = ["X1", "Y1", "X2", "Y2", "name_of_line"]
with open("Swip_line_Data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(fields)
    writer.writerows(lines)
