import matplotlib.pyplot as plt

# Line data: (x1, y1, x2, y2, name)
segments = [
    (187,184,16,111,"s0"),
    (92,146,128,145,"s1"),
    (40,66,9,69,"s2"),
    (69,154,70,171,"s3"),
    (0,116,30,59,"s4"),
    (81,79,35,85,"s5"),
    (105,63,15,45,"s6"),
    (186,166,130,13,"s7"),

]

plt.figure(figsize=(8, 8))

for x1, y1, x2, y2, name in segments:
    plt.plot([x1, x2], [y1, y2], marker='o')
    mx, my = (x1 + x2) / 2, (y1 + y2) / 2
    plt.text(mx, my, name, fontsize=9)

plt.xlabel("X")
plt.ylabel("Y")
plt.title("Line Segment Visualization")
plt.grid(True)
plt.axis("equal")   # VERY important for geometry
plt.show()
