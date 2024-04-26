import re
import os
import sys
import linecache
import numpy as np
import matplotlib.pyplot as plt 

""" 
This program is currently a quick script and not a well designed code/tool 
it may have issues in it's structure
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

# Operators in this project implementation
OPERATORS = ["MAE", "MAPE", "RIV", "RMSE"]

# Criterias in this project implementation
criterias = ["UD", "P", "Q", "RV"]

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

#
def generate_title(letter):
    str = "Influence of the depth on the trees predictions\n Splitting criteria is : "
    criteria = ""
    if letter == criterias[0]:
        criteria = "Uniform Distribution"
    elif letter == criterias[1]:
        criteria = "Percentiles"
    elif letter == criterias[2]:
        criteria = "Quartiles"
    elif letter == criterias[3]:
        criteria = "Random Values"
    else:
        print(f"{RED} Couldn't generate title, variable unknown {RESET}")
        return " Default title"
    return str+criteria

#
def find_operators(text):
    found_operators = []
    for operator in OPERATORS:
        if operator in text:
            found_operators.append(operator)
    return found_operators

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
    def __init__(self, folder_path, operator : str):
        if not os.path.isdir(folder_path):
            print(f"{RED} Folder path does not exist. {RESET}")
        
        self.operator = operator
        self.file_data = {"UD": [], "P": [], "Q": [], "RV": []}
        # Parse files in the folder
        self.parse_files(folder_path)

    # Getter for operator
    def get_operator(self):
        return self.operator

    # Getter for infos
    def get_file_data(self):
        return self.file_data

    # Function to extract the number from the file name
    def get_file_number(self, file_name):
        return int(file_name.split('_')[-1].split('.')[0])

    # Function that iterates through the files in the folder
    def parse_files(self, folder_path):
        files = os.listdir(folder_path)
        sorted_files = sorted(files, key=lambda x : self.get_file_number(x))
        #Sort the files so it correctly put them in the arrays for plotting
        for file_name in sorted_files:
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
                self.file_data[prefix].append(data)

    # Function that reads the files and get : runtime, MAE result, MAPE result, std dev
    def get_infos_from_file(self, file_path):
        
        # Hard coded line getters a bit ugly tho
        runtime_line = linecache.getline(file_path, RUNTIME_LINE)
        last_line = linecache.getline(file_path, LAST_LINE)

        runtime_s = self.parse_runtime(runtime_line)

        unoptimal_reader = last_line.split()
        mae = float(unoptimal_reader[-3])
        mape = float(unoptimal_reader[-2])
        std_dev = float(unoptimal_reader[-1])

        return ProgramData(mae,mape,std_dev,runtime_s) 

    #
    def parse_runtime(self, line):
        runtime_pattern = r"run time:= (\d+)h, (\d+)m, (\d+)s|run time:= (\d+)(ms)"

        match = re.search(runtime_pattern, line)
        if match:
            if match.group(1) and match.group(2) and match.group(3):
                # If hours, minutes, and seconds are present
                hours = convert_to_seconds(int(match.group(1)), "h")
                minutes = convert_to_seconds(int(match.group(2)),"m")
                seconds = int(match.group(3))
                total_seconds = hours + minutes + seconds
            elif match.group(4):
                total_seconds =convert_to_seconds(int(match.group(4)),"ms")

            else:
                total_seconds = 0  # Default to 0 if no match
            return total_seconds
        else:
            return None


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

#
def plot(mae_arr, mape_arr, riv_arr, rmse_arr, depth_arr, crit, eval_metric) :
    """
    Take as input an np.array and the infos of the hyperparameters of the model we ran
    and displayes it using matplotlib plots
    """

    # WE NEED TO HAVE A VECTOR OF DEPTH
    title = generate_title(crit)
    file_save_name = eval_metric+"_"+crit+".pdf"

    plt.figure()
    plt.title(title)

    #MAE plot
    plt.plot(depth_arr, mae_arr, "-x", color="red", label="Operator : MAE")
    #MAPE plot
    plt.plot(depth_arr, mape_arr,"-x", color="green", label="Operator: MAPE")
    #RIV plot
    plt.plot(depth_arr, riv_arr, "-x", color="blue", label="Operator : RIV")
    #MSE plot
    plt.plot(depth_arr, rmse_arr, "-x", color="black", label="Operator : RMSE")
    
    plt.xlabel("Tree Depth")
    if(eval_metric == "runtime"):
        plt.ylabel(eval_metric+" (s)")
    else:
        plt.ylabel(eval_metric) 
    
    plt.legend()
    #plt.show()
    plt.savefig(file_save_name)
    
#
def handle_results(data_array, depth_arr, crit):
    """
    Take as input an array of arrays of data and for each corresponding tuple element 
    of the array it plots them using the folowing logic
    """
    mae_array = []
    mape_array = []
    std_dev_array = []
    runtime_array = []

    for array in data_array:
        mae_curr_model = []
        mape_curr_model =[]
        stdddev_curr_model =[]
        runtime_curr_model =[]

        for data in array:
            mae_curr_model.append(data.get_mae())
            mape_curr_model.append(data.get_mape())
            stdddev_curr_model.append(data.get_std_dev())
            runtime_curr_model.append(data.get_runtime())

        mae_array.append(np.array(mae_curr_model))
        mape_array.append(np.array(mape_curr_model))
        std_dev_array.append(np.array(stdddev_curr_model))
        runtime_array.append(np.array(runtime_curr_model))

    # For each Operator using the same criteria 
    plot(mae_array[0], mae_array[1], mae_array[2], mae_array[3], depth_arr, crit, "mae") 
    plot(mape_array[0], mape_array[1], mape_array[2], mape_array[3], depth_arr, crit, "mape")
    plot(std_dev_array[0], std_dev_array[1], std_dev_array[2], std_dev_array[3], depth_arr, crit, "std_dev")
    plot(runtime_array[0], runtime_array[1], runtime_array[2], runtime_array[3], depth_arr, crit, "runtime")

# Handler function
def main(path, depth):

    found_folders = find_folders(path, bench_dirs)
    depth_arr = np.arange(1, depth + 1)
    results = [None] * len(found_folders)

    i = 0
    if found_folders and len(found_folders) == 4:
        print(f"{GREEN} Found the following benchmarks folders: {RESET}")
        for folder in found_folders:
            print(folder)
            operator = find_operators(folder)
            results[i] = Parser(folder, operator) 
            i+=1 
    else:
        print(f"{RED} No benchs folders found...{RESET}")

    # Not sure either
    
    p_array = []
    q_array = []
    ud_array = []
    rv_array = []

    # Not quite what we need atm
    for obj in results:
        p_array.append(obj.get_file_data()["P"])
        q_array.append(obj.get_file_data()["Q"])
        rv_array.append(obj.get_file_data()["RV"])        
        ud_array.append(obj.get_file_data()["UD"]) 

    handle_results(p_array,depth_arr, "P")
    handle_results(q_array, depth_arr, "Q")
    handle_results(rv_array, depth_arr, "RV")
    handle_results(ud_array, depth_arr, "UD") 


#
if __name__ == "__main__":
    # Check if directory path is provided as argument
    if len(sys.argv) != 3:
        script_name = os.path.basename(__file__)
        print(f"Usage is: python3 {script_name} <path_to_benchmark_dir> <highest_depth>")
        sys.exit(1)

    main(sys.argv[1], int(sys.argv[2]))