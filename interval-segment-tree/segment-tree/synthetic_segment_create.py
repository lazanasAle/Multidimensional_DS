import random as rnd

Size_of_dataset = 20000
Upper_bound = 200

numbers = [int(rnd.uniform(0, Upper_bound)) for i in range(Size_of_dataset)]

fd = open("dataset.txt", "w")

for n in numbers:
    fd.write(f"{n}\n")

fd.close()
