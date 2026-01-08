from InquirerPy import inquirer
import json
import subprocess
from pathlib import Path

DIMENSIONS=["r (longitude/x)", "u (latitude/y)", "t (time)"]

print("=" *60)
print("3D R-Tree Interactive Query System")
print("=" * 60)
    
choices = [(i, dim) for i, dim in enumerate(DIMENSIONS)]

index_dims = inquirer.checkbox(
    message="Select dimensions for your query (space to select, DO NOT PRESS ENTER until finished):",
    choices=choices,
    validate=lambda x: len(x) > 0 or "Please select at least one dimension"
).execute()

idx_dims = [dim[0] for dim in index_dims]

#build the range query
range_query = {}

for dim in idx_dims:
    print(f"\n--- Setting bounds for {DIMENSIONS[dim]} ---")
    
    #dimensions can also be floats (coordinates and time)
    low = float(inquirer.number(
        message=f"Enter lower bound for {DIMENSIONS[dim]}:",
        float_allowed=True
    ).execute())
    
    high = float(inquirer.number(
        message=f"Enter upper bound for {DIMENSIONS[dim]}:",
        float_allowed= True
    ).execute())
    
    if low > high:
        raise ValueError(f"{DIMENSIONS[dim]} lower bound must be less than upper bound")
    
    range_query[str(dim)] = [low, high]

data_to_read = int(inquirer.number(
    message="Select how many rows to read:",
    float_allowed=False
).execute())

config = {
    "index_dimensions": idx_dims,
    "range_query": range_query
}

config_path = Path("rtree_query.json")

config_path.write_text(json.dumps(config))

result = subprocess.run(["./rtree_from_json", f"{data_to_read}"], capture_output =True, text=True)

print(result.stdout)