import csv
import random
from dataclasses import fields
from itertools import combinations


def intersection(line1, line2):
    x1, y1, x2, y2 = line1[:4]
    x3, y3, x4, y4 = line2[:4]

    denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)

    # Αν ο παρονομαστής είναι 0, οι γραμμές είναι παράλληλες ή ταυτίζονται
    if denom == 0:
        return None

    # Υπολογισμός των παραμέτρων t και u (Cramer's rule)
    t_num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)
    u_num = (x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)

    t = t_num / denom
    u = -(u_num / denom)

    # Έλεγχος αν το σημείο τομής ανήκει ΚΑΙ ΣΤΑ ΔΥΟ τμήματα
    # Για τομές μόνο στα εσωτερικά σημεία (όχι άκρα), χρησιμοποιήστε 0 < t < 1
    if 0 <= t <= 1 and 0 <= u <= 1:
        intersect_x = x1 + t * (x2 - x1)
        intersect_y = y1 + t * (y2 - y1)
        return (intersect_x, intersect_y)

    return None


def are_collinear(p1, p2, p3):
    return (p2[0] - p1[0]) * (p3[1] - p1[1]) == (p2[1] - p1[1]) * (p3[0] - p1[0])


def Collinear(line1, line2):
    p1, p2 = (line1[0], line1[1]), (line1[2], line1[3])
    p3, p4 = (line2[0], line2[1]), (line2[2], line2[3])
    return are_collinear(p1, p2, p3) and are_collinear(p1, p2, p4)


Size_of_dataset = 20
Upper_bound = 200  # the upper bound for thr interval
intersection_points = []

lines = []

# the checking for the algorithm
while len(lines) < Size_of_dataset:
    x1, y1 = int(random.uniform(0, Upper_bound)), int(random.uniform(0, Upper_bound))
    x2, y2 = int(random.uniform(0, Upper_bound)), int(random.uniform(0, Upper_bound))
    name = f"s{len(lines)}"
    segment_created = [x1, y1, x2, y2, name]
    current_intersections = []
    if x1 == x2 and y1 == y2:
        continue
    # check for collinearity
    valid = True
    if len(lines) >= 1:
        it = len(lines) - 1

        for point in intersection_points:
            # Έλεγχος αν το άκρο (x1, y1) ή το (x2, y2) ταυτίζεται με υπάρχουσα τομή
            if (abs(x1 - point[0]) < 1e-9 and abs(y1 - point[1]) < 1e-9) or (
                abs(x2 - point[0]) < 1e-9 and abs(y2 - point[1]) < 1e-9
            ):
                valid = False
                break

        for j in range(len(lines)):
            if not (Collinear(segment_created, lines[j])):
                # no three line intersect at a single point
                interpoint = intersection(segment_created, lines[j])
                if interpoint:
                    if any(
                        abs(interpoint[0] - p[0]) < 1e-9
                        and abs(interpoint[1] - p[1]) < 1e-9
                        for p in intersection_points
                    ):
                        valid = False
                        break
                    else:
                        current_intersections.append(interpoint)
            else:
                valid = False
                break

        if valid == True:
            intersection_points.extend(current_intersections)
            lines.append(segment_created)

    else:
        lines.append(segment_created)  # first point inserted


fields = ["X1", "Y1", "X2", "Y2", "name_of_line"]
with open("Swip_line_Data.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(fields)
    writer.writerows(lines)
