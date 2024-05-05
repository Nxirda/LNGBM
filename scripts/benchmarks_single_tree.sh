#!/bin/bash

#Check number of args
if [ $# != 3 ]; then
    echo "Usage is: $0 <path to bin> <path_to_dataset> <max_depth>"
    exit 1;
fi

program="$1"
dataset="$2"

# Define the range of depth for trees values
start_dt=1
end_dt="$3"

result_dir="results"

# Remove the existing log file if it exists
if [ -d "$result_dir" ]; then
    read -p "Directory '$result_dir' already exists. Do you want to clean it? (y/n): " confirm
    if [ "$confirm" = "y" ]; then
        echo "Cleaning existing result directory: $result_dir"
        rm -rf "$result_dir/*"
    else 
        echo "Skipping directory cleanup."
    fi
else 
    mkdir -p -v "$result_dir"
fi

log_file="$result_dir/script_log.txt"

# Remove the existing log file if it exists
if [ -f "$log_file" ]; then
    echo "Cleaning existing log file: $log_file"
    rm -f "$log_file"
fi

operator=("MAE" "MAPE" "RIV" "RMSE")
criteria=("UD" "P" "Q" "RV") #"UV")

algorithm="h"

# For these tests 
for op in "${operator[@]}"; do

    output_dir="$result_dir/$op"_benchs
    # Remove the directory if it already exists
    if [ -d "$output_dir" ]; then
        read -p "Directory '$output_dir' already exists. Do you want to clean it? (y/n): " confirm
        if [ "$confirm" = "y" ]; then
            echo "Cleaning existing directory: $output_dir"
            rm -v -f "$output_dir/"*.data
        else
            echo "Skipping directory cleanup."
        fi
    fi

    mkdir -p -v "$output_dir"
    
    for crit in "${criteria[@]}"; do
        for ((dt=start_dt; dt<=end_dt; dt++)); do
            
            output_file="${crit}_${dt}.data"
            echo -e "\e[1;32m Running program with config: -dt=$dt -o=$op -c=$crit -cv=5 \e[0m" >> "$log_file"
            ./"$program" "$dataset" "-dt=$dt" "-o=$op" "-c=$crit" "-nt=1" "-cv=5" > "$output_dir/$output_file" 

            if [ $? -eq 0 ]; then
                echo -e "\e[1;33m [Info] Program execution complete. \e[0m"
            else
                echo -e "\e[1;31m [Error]: Program execution failed. \e[0m"
                exit 1
            fi
        
            # Verify if writing to the file ended correctly
            if [ -s "$output_dir/$output_file" ]; then
                echo -e "\e[1;33m [Info] Writing to file completed successfully. \e[0m" >> "$log_file"
            else
                echo -e "\e[1;31m [Error]: Writing to file failed. \e[0m" >> "$log_file"
                exit 1
            fi
        done
    done
done     

# Histogram bench part

histogram_out_dir="$result_dir/H_benchs"
# Remove the directory if it already exists
if [ -d "$histogram_out_dir" ]; then
    read -p "Directory '$histogram_out_dir' already exists. Do you want to clean it? (y/n): " confirm
    if [ "$confirm" = "y" ]; then
        echo "Cleaning existing directory: $histogram_out_dir"
        rm -v -f "$histogram_out_dir/"*.data
    else
        echo "Skipping directory cleanup."
    fi
else
    mkdir -v -p "$histogram_out_dir"
fi

for ((dt=start_dt; dt<=end_dt; dt++)); do
    output_file="H_${dt}.data"
    echo -e "\e[1;32m Running program with config: -dt=$dt -alg=h -cv=5 \e[0m" >> "$log_file"
    ./"$program" "$dataset" "-dt=$dt" "-alg=h" "-nt=1" "-cv=5" > "$histogram_out_dir/$output_file" 

    if [ $? -eq 0 ]; then
        echo -e "\e[1;33m [Info] Program execution complete. \e[0m"
    else
        echo -e "\e[1;31m [Error]: Program execution failed. \e[0m"
        exit 1
    fi

    # Verify if writing to the file ended correctly
    if [ -s "$histogram_out_dir/$output_file" ]; then
        echo -e "\e[1;33m [Info] Writing to file completed successfully. \e[0m" >> "$log_file"
    else
        echo -e "\e[1;31m [Error]: Writing to file failed. \e[0m" >> "$log_file"
        exit 1
    fi
done

# Running the python script
dependency=plot_one_tree_benchmarks.py
if [ -f "$dependency" ]; then
    echo -e "\e[1;33m [Info] Plotting script found : $dependency \e[0m"
    python3 "$dependency" "./$result_dir" "$end_dt"
else 
    echo -e "\e[1;31m [Error]: Plotting script file not found : $dependency \e[0m"
fi