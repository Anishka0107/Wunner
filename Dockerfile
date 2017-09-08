FROM ubuntu:xenial

RUN apt-get update && apt-get install -y \
build-essential \
cmake \
libboost-dev \
wget

WORKDIR /home/Wunner

COPY ${PWD} .

RUN mkdir -p build

RUN cd build && cmake ..
RUN cd build && make

ENV PATH $PATH:/home/Wunner/build/bin
