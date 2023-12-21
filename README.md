# LAPACK DGTERF performance prediction via Machine Learning

## Build : Dependencies

You will need gtest 

```sh
sudo apt-get install libgtest-dev
```

## Build :

```sh
git clone https://github.com/Nxirda/PPN_Projet_MEM.git
cd PPN_Projet_MEM && mkdir build
cd build && cmake ..
make -j
```

## Usage :

```sh
# Ensure the project has been built properly
cd build
./main [Path to DataSet] [Split Metric] [Depth] [Number of Trees]
```

## Tests : 

If you want to make sure everything has been done properly you
can run the tests :

- Refer to the Readme in the test directory

## Benchmarks : 

If you want to benchmark your model you can run the benchmarks :

- Refer to the Readme in the Benchmark directory


## Authors :
[Adrien Henrot](https://github.com/Nxirda) \
[Valentin Denis](https://github.com/valde3003)  \
[Alioune Badara](https://github.com/Alioune-Badara)  

## Thanks to

[Mathys Jam](https://github.com/Thukisdo), for guiding us through the project
