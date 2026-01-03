from InquirerPy import inquirer
import json
import subprocess
from pathlib import Path

DIMENSIONS = ["Budget", "Revenue", "Popularity", "Vote Average", "runtime", "Vote Count"]

choices = [(i, dim) for i, dim in enumerate(DIMENSIONS)]

index_dims = inquirer.checkbox(
    message="Select Dimensions that you want to be indexed (space for select  DO NOT PRESS ENTER until you finish selecting):",
    choices=choices,
    validate=lambda x: len(x) > 0 or "Please select at least one dimension"
).execute()

idx_dims = [dim[0] for dim in index_dims]

range_query = {}
for dim in idx_dims:
    flt_allowed = (dim <= 3)
    caster = float if flt_allowed else int

    low = caster(inquirer.number(
        message=f"Select lower bound for:{DIMENSIONS[dim]} dimension:",
        float_allowed=flt_allowed
    ).execute())

    high = caster(inquirer.number(
        message=f"Select upper bound for:{DIMENSIONS[dim]} dimension:",
        float_allowed=flt_allowed
    ).execute())

    if low > high:
        raise ValueError(f"{DIMENSIONS[dim]} lower bound must be less than upper bound")

    range_query[str(dim)] = [low, high]


skyline_query = []

for dim in idx_dims:

    choice = inquirer.select(
        message=f"Select the criterion for best {DIMENSIONS[dim]} dimension:",
        choices=["min", "max"]
    ).execute()

    skyline_query.append(choice)


data_to_read = int(inquirer.number(
    message="Select how many rows to read:",
    float_allowed=False
).execute())

config = {
    "index_dimensions": idx_dims,
    "range_query": range_query,
    "skyline_query": skyline_query
}

config_path = Path("query.json")

config_path.write_text(json.dumps(config))

result = subprocess.run(["./from_json", f"{data_to_read}"], capture_output=True,text=True)

print(result.stdout)







