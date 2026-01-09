import math

import pandas as pd
from pyproj import Transformer

# setup for lat long to x y conversion
transformer = Transformer.from_crs("epsg:4326", "epsg:3857")
# csv constants for reading the csv
const_cols = ["aircraft_id", "year", "month", "day", "hour", "minute", "second"]
change_cols = ["latitude", "longitude"]
keep_cols = const_cols + change_cols

df = pd.read_csv("tartanaviation_adsb_900k.csv", usecols=keep_cols)
n_data = []

i = 0
for index, row in df.iterrows():
    n_data.append({})
    for col in const_cols:
        n_data[i][col] = row[col]
    lon = row["longitude"]
    lat = row["latitude"]
    x, y = transformer.transform(lon, lat)
    r, u = math.sqrt(x * x + y * y), math.atan(y / x)
    n_data[i]["r"] = r
    n_data[i]["u"] = u
    i += 1

data_frame = pd.DataFrame(n_data)

data_frame.to_csv("flight_data_readable.csv", index=False)

print("Data written to flight_data_readable.csv")
