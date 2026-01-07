import csv

import matplotlib.pyplot as plt

segments = []

# Άνοιγμα του αρχείου για ανάγνωση
with open("Swip_line_Data.csv", mode="r", encoding="utf-8") as file:
    reader = csv.reader(file)
    next(reader)
    for row in reader:
        # row είναι μια λίστα από strings: ['196', '96', '109', '30', 's0']
        # Μετατροπή των πρώτων 4 στοιχείων σε int και διατήρηση του 5ου ως string
        if not row:
            continue
        segment = (int(row[0]), int(row[1]), int(row[2]), int(row[3]), row[4])
        segments.append(segment)

# Line data: (x1, y1, x2, y2, name)


plt.figure(figsize=(8, 8))

for x1, y1, x2, y2, name in segments:
    plt.plot([x1, x2], [y1, y2], marker="o")
    mx, my = (x1 + x2) / 2, (y1 + y2) / 2
    plt.text(mx, my, name, fontsize=9)

plt.xlabel("X")
plt.ylabel("Y")
plt.title("Line Segment Visualization")
plt.grid(True)
plt.axis("equal")  # VERY important for geometry
plt.show()
