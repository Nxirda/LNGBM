import re
import os
import sys
import linecache
#import numpy as np
#import matplotlib.pyplot as plt 

""" 
This program is currently a quick script and not a well designed code/tool 
it may have issues in it's structure at the moment 
"""

# ANSI escape codes for colors
GREEN = "\033[92m"
RED = "\033[91m"
RESET = "\033[0m"

# Constants for line position we need in this project
RUNTIME_LINE = 10
LAST_LINE = 17

# Folders representing operators
bench_dirs = ["MAE_benchs", "MAPE_benchs", "RIV_benchs", "RMSE_benchs"]

# Criterias in this project implementation
criterias = ["H", "P", "Q", "RV"]

#
def convert_to_seconds(value, unit):
    if unit == 'ms':
        return value / 1000.0
    elif unit == 's':
        return value
    elif unit == 'm':
        return value * 60
    elif unit == 'h':
        return value * 3600

# Store infos for one config that has been run
class ProgramData:
    def __init__(self, mae, mape, std_dev, runtime):
        self.mae = mae
        self.mape = mape
        self.std_dev = std_dev
        self.runtime = runtime

    def get_mae(self):
        return self.mae

    def get_mape(self):
        return self.mape

    def get_std_dev(self):
        return self.std_dev

    def get_runtime(self):
        return self.runtime

# Store model infos for plotting
class Parser:
    #Constructor
    def __init__(self, folder_path):
        if not os.path.isdir(folder_path):
            print(f"{RED} Folder path does not exist. {RESET}")
        
        self.file_data = {"H": [], "P": [], "Q": [], "RV": []}

        # Parse files in the folder
        self.parse_files(folder_path)

    # Getter for infos
    def get_file_data(self):
        return self.file_data

    # Function that iterates through the files in the folder
    def parse_files(self, folder_path):
        for file_name in os.listdir(folder_path):
            file_path = os.path.join(folder_path, file_name)
            if os.path.isfile(file_path):
                self.process_file(file_path)
    
    # Function that processes the files in the folder 
    def process_file(self, file_path):
        # Just ensure we read a correct file
        file_name = os.path.basename(file_path)
        file_parts = file_name.split('_')
        if len(file_parts) == 2 and file_parts[1].endswith('.data'):
            prefix, number_ext = file_parts[0], file_parts[1]
            number = number_ext.split('.')[0]
            if prefix in criterias:
                # Read data from the file and store it in the object
                data = self.get_infos_from_file(file_path)
                self.file_data[prefix].append((number, data))

    # Function that reads the files and get : runtime, MAE result, MAPE result, std dev
    def get_infos_from_file(self, file_path):
        
        # Hard coded line getters a bit ugly tho
        runtime_line = linecache.getline(file_path, RUNTIME_LINE)
        last_line = linecache.getline(file_path, LAST_LINE)

        runtime_pattern = r"run time:= (\d+)(ms|s|m|h)"
        
        runtime_match = re.search(runtime_pattern, runtime_line)
        runtime_value = runtime_match.group(1)
        runtime_unit = runtime_match.group(2)
        runtime_s = convert_to_seconds(int(runtime_value), runtime_unit)

        unoptimal_reader = last_line.split()
        mae = float(unoptimal_reader[-3])
        mape = float(unoptimal_reader[-2])
        std_dev = float(unoptimal_reader[-1])

        return ProgramData(mae,mape,std_dev,runtime_s) 


# Get the folders path
def find_folders(directory, folders_to_find):
    found_folders = []

    if not os.path.exists(directory):
        return found_folders
    
    for folder in folders_to_find:
        folder_path = os.path.join(directory, folder)
        if os.path.exists(folder_path) and os.path.isdir(folder_path):
            found_folders.append(folder_path)
    
    return found_folders
    
# Handler function
def main(path):
    found_folders = find_folders(path, bench_dirs)

    results = [None] * len(found_folders)
    i = 0
    if found_folders:
        print(f"{GREEN} Found the following benchmarks folders: {RESET}")
        for folder in found_folders:
            print(folder)
            results[i] = Parser(folder).get_file_data()
            i+=1 
    else:
        print(f"{RED} No benchs folders found...{RESET}")

    # Not sure either
    H_array = []
    P_array = []
    Q_array = []
    RV_array = []
    
    # Not quite what we need atm
    for obj in results:
        H_array.extend(obj["H"])
        P_array.extend(obj["P"])
        Q_array.extend(obj["Q"])
        RV_array.extend(obj["RV"])

    # temporary ofc
    print("H_array:", H_array)
    print("P_array:", P_array)
    print("Q_array:", Q_array)
    print("RV_array:", RV_array)


if __name__ == "__main__":
    # Check if directory path is provided as argument
    if len(sys.argv) != 2:
        script_name = os.path.basename(__file__)
        print(f"Usage is: python3 {script_name} <path_to_benchmark_dir>")
        sys.exit(1)

    main(sys.argv[1])