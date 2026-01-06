import matplotlib.pyplot as plt

# Line data: (x1, y1, x2, y2, name)
segments = [
    (155, 136, 190, 32, "s0"),
    (147, 150, 105, 120, "s1"),
    (145, 30, 38, 115, "s2"),
    (102, 100, 43, 187, "s3"),
    (10, 15, 106, 41, "s4"),
    (165, 177, 75, 160, "s5"),
    (197, 155, 1, 91, "s6"),
    (61, 93, 127, 135, "s7"),
    (42, 34, 8, 117, "s8"),
    (90, 57, 96, 70, "s9"),
    (73, 30, 97, 173, "s10"),
    (159, 119, 83, 99, "s11"),
    (145, 83, 154, 93, "s12"),
    (115, 74, 51, 102, "s13"),
    (129, 48, 73, 13, "s14"),
    (14, 82, 147, 94, "s15"),
    (68, 142, 85, 68, "s16"),
    (49, 166, 166, 42, "s17"),
    (68, 152, 44, 71, "s18"),
    (2, 78, 113, 6, "s19"),
]

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
