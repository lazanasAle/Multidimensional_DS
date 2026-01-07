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

    #query type selection
    query_type= inquirer.select(
        message="Select query type:",
        choices=[
            {"name": "Range Query", "value": "range"},
            {"name": "Full Dataset Statistics", "value": "stats"}
        ]
    ).execute()

    print(f"\nSelecter query type: {query_type}")
    if query_type =="stats":
        config ={
            "csv_path": csv_path,
            "query_type": "stats"
        }
        print("\nStatistics mode")
    else:
        #Spatial range (X=Longitude)
        print("\n----- Spatial X Coordinate (Longitude/r) -----")
        
        x_min= float(inquirer.number(
            message="Enter min X (longitude):",
            float_allowed= True
        ).execute())

        x_max= float(inquirer.number(
            message="Enter max X (longitude):",
            float_allowed= True,
            validate=lambda val: float(val) >= x_min or "Max must be >= min"
        ).execute())

        #Spatial range (Y=Latitude)
        print("\n----- Spatial Y Coordinate (Latitude/u) -----")
        
        y_min= float(inquirer.number(
            message="Enter min Y (longitude):",
            float_allowed= True
        ).execute())

        y_max= float(inquirer.number(
            message="Enter max Y (longitude):",
            float_allowed= True,
            validate=lambda val: float(val) >= y_min or "Max must be >= min"
        ).execute())

        #Temporal range
        print("\n----- Time Range (Timestamp) -----")
        
        t_min= float(inquirer.number(
            message="Enter min timestamp:",
            float_allowed= True
        ).execute())

        t_max= float(inquirer.number(
            message="Enter max timestamp:",
            float_allowed= True,
            validate=lambda val: float(val) >= t_min or "Max must be >= min"
        ).execute())

        max_results=int(inquirer.number(
            message="Max results to display:",
            default=10,
            float_allowed=False
        ).execute())

        show_stats= inquirer.confirm(
            message="Show dataset statistics?",
            default=True
        ).execute()

        #Build configuration
        config={
            "csv_path": csv_path,
            "query_type": "range",
            "range_query": {
                "x": [x_min, x_max],
                "y": [y_min, y_max],
                "t": [t_min,t_max]
            },
            "options": {
                "max_results": max_results,
                "show_stats": show_stats
            }
        }

    #Save configuration
    config_path=Path("rtree_query.json")
    config_path.write_text(json.dumps(config, indent=2))
    print(f"\nConfiguration Saved to {config_path}")
    print(f"  Config: {json.dumps(config, indent=2)}")

    #execute C++ program
    print("\n"+ "=" *60) 
    print("Executing R-Tree Query...")
    print("=" *60 +"\n")       

try:
    result= subprocess.run(
        ["./test_rtree_json"],
        capture_output=True,
        text=True,
        timeout=60
    )

    #print results
    print(result.stdout)

    if result.stderr:
        print("Errors/Warnings:", result.stderr)
    
    if result.returncode!=0:
        print(f"\nProgram exited with code {result.returncode}")
except FileNotFoundError:
    print("Error: ./test_rtree_json not found!")
    print("Please compile the C++ program first:")
    print("  g++ -o test_rtree_json test_rtree_json.cpp -std=c++17")
    print(" or")
    print(" make test_rtree_json")
except subprocess.TimeoutExpired:
    print("Error: Query timed out after 1 minute")
except Exception as e:
    print(f"Error executing query: {e}")


if __name__ == "__main__":
    main()