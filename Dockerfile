FROM debian:12

LABEL maintainer="Nxirda <nerTren.@protonmail.com>" \
	 description="Dockerfile for building this project with minimal tools"

#Dependencies
RUN apt-get update -y && \
    apt install g++ -y && \
    apt install build-essential -y && \
    apt install cmake -y && \
    apt install vim -y && \
    apt install libgtest-dev -y && \
    apt install mpich -y 

#Environment 
VOLUME /root/env
WORKDIR /root/env

COPY . .

ENV HOSTNAME=docker

#Build the project
RUN mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
	make -j 8