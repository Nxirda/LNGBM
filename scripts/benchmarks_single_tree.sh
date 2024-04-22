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

operator=("MAE" "MAPE" "RIV" "RMSE")
criteria=("H" "P" "Q" "RV") #"UV")

#For these tests nt
for op in "${operator[@]}"; do

    output_dir="$op"_benchs
    mkdir -p -v "$output_dir"
    
    for crit in "${criteria[@]}"; do
        for ((dt=start_dt; dt<=end_dt; dt++)); do
            
            output_file="${crit}_${dt}.data"
            echo -e "\e[1;32m Running program with config: -dt=$dt -o=$op -c=$crit -cv=5 \e[0m"
            ./"$program" "$dataset" "-dt=$dt" "-o=$op" "-c=$crit" "-nt=1" "-cv=5" > "$output_dir/$output_file" 
            echo -e "\e[1;33m Program execution complete. \e[0m"
        done
    done
done                                                  