from InquirerPy import inquirer
import json
import subprocess
from pathlib import Path

# Default CSV file path
DEFAULT_CSV = "flight_data_readable.csv"

def main():
    print("=" *60)
    print("3D R-Tree Interactive Query System")
    print("=" * 60)

    #select csv file
    csv_path= inquirer.text(
        message="Enter CSV File Path:",
        default=DEFAULT_CSV,
        validate= lambda x: Path(x).exists() or f"File {x} doesn't exist"
    ).execute()


