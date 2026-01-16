import pandas as pd
import matplotlib.pyplot as plt

# Read all points
points_df = pd.read_csv("test.csv")  # All points
hull_df = pd.read_csv("export_data.csv")  # Convex hull points in order

# Plot all points
plt.scatter(points_df['budget'], points_df['popularity'], color='blue', label='All Points')

# Plot convex hull points
plt.scatter(hull_df['budget'], hull_df['popularity'], color='red', label='Convex Hull Points')

# Connect convex hull points to form the polygon
plt.plot(list(hull_df['budget']) + [hull_df['budget'][0]],
         list(hull_df['popularity']) + [hull_df['popularity'][0]],
         color='red', linestyle='--')

# Labels and title
plt.xlabel('Budget')
plt.ylabel('Popularity')
plt.title('Points and Convex Hull Visualization')
plt.legend()
plt.grid(True)
plt.show()
