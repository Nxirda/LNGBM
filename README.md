# Machine Learning : regression tool

    This project has been developped in order to predict the performance of the DGETRF core from the LAPACK library

## Build with docker :

### Docker :

You will need docker installed on your machine

On Debian/Ubuntu with apt :

```sh
$ sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

#Make sure install went right :
$ sudo docker run hello-world
```

Docker documentation is available [here](https://docs.docker.com/engine/install/)

### Build the project :

```sh
#Clone this repository
$ git clone https://github.com/Nxirda/PPN_Projet_MEM.git

#First build the image, once in the project directory :
$ docker build . -t project_mem 

#Then start the container, the project will be built at the same time
$ docker run --rm -it project_mem
```

Then you can skip to the Usage section below


## Build : Standard way

### Dependencies :

You will need :

Gtest : 

```sh
sudo apt install libgtest-dev
```

MPI :

Boost : 

### Build :

```sh
$ git clone https://github.com/Nxirda/PPN_Projet_MEM.git
$ cd PPN_Projet_MEM && mkdir build
$ cd build && cmake ..
$ make -j
```

If your installation of MPI isnt in your path you can run the following command
instead of "cmake .."

```sh
$ cmake -DCMAKE_CXX_COMPILER=/path/to/your/mpi/c++/compiler -DCMAKE_PREFIX_PATH=/path/to/your/mpi/installation ..
```

```sh
$ cmake -S . -B <BUILD_DIR> <CMAKE OPTIONS>
```

## Usage :

```sh
# Ensure the project has been built properly
cd build
Usage is : mpiexec -np <Process>  \
           ./main [Path to DataSet] [Split Metric] [Split Criteria] [Depth] [Number of Trees] \
           [Cross_Val] [Folds]
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
