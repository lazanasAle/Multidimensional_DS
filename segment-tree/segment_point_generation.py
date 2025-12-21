import random as rnd

data_size = 100000
upper_bound = 300


points = [int(rnd.uniform(0, upper_bound)) for i in range(data_size)]

f = open('points.txt', 'w')
for point in points:
    f.write(str(point) + '\n')

f.close()

