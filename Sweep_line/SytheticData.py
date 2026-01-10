import csv
import random
import sys
import time


def bbox_overlap(l1, l2):
    return not (
        max(l1[0], l1[2]) < min(l2[0], l2[2])
        or max(l2[0], l2[2]) < min(l1[0], l1[2])
        or max(l1[1], l1[3]) < min(l2[1], l2[3])
        or max(l2[1], l2[3]) < min(l1[1], l1[3])
    )


def collinear(l1, l2):
    x1, y1, x2, y2 = l1[:4]
    x3, y3, x4, y4 = l2[:4]
    dx = x2 - x1
    dy = y2 - y1
    return dx * (y3 - y1) == dy * (x3 - x1) and dx * (y4 - y1) == dy * (x4 - x1)


def intersection(l1, l2):
    x1, y1, x2, y2 = l1[:4]
    x3, y3, x4, y4 = l2[:4]

    denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    if denom == 0:
        return None

    t_num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)
    u_num = (x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)

    t = t_num / denom
    u = -u_num / denom

    if 0 <= t <= 1 and 0 <= u <= 1:
        return (x1 + t * (x2 - x1), y1 + t * (y2 - y1))
    return None


Size_of_dataset = int(sys.argv[1])
Upper_bound = int(sys.argv[2])

lines = []
intersection_points = set()
start = time.perf_counter()
while len(lines) < Size_of_dataset:
    x1 = random.randrange(Upper_bound)
    y1 = random.randrange(Upper_bound)
    x2 = random.randrange(Upper_bound)
    y2 = random.randrange(Upper_bound)

    if x1 == x2 and y1 == y2:
        continue

    # reject if endpoint already used as intersection
    if (x1, y1) in intersection_points or (x2, y2) in intersection_points:
        continue

    segment = [x1, y1, x2, y2, f"s{len(lines)}"]
    valid = True
    new_intersections = []

    for other in lines:
        if not bbox_overlap(segment, other):
            continue

        if collinear(segment, other):
            valid = False
            break

        p = intersection(segment, other)
        if p:
            key = (round(p[0], 9), round(p[1], 9))
            if key in intersection_points:
                valid = False
                break
            new_intersections.append(key)

    if valid:
        lines.append(segment)
        intersection_points.update(new_intersections)


end = time.perf_counter()
print(f"Total time: {end - start:.4f} seconds")
fields = ["X1", "Y1", "X2", "Y2", "name_of_line"]
with open("Swip_line_Data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(fields)
    writer.writerows(lines)
