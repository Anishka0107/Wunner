FROM alpine:3.6

RUN apk add --update --no-cache \
g++ \
make \
cmake \
boost-dev \
wget

WORKDIR /home/Wunner

COPY ${PWD} .

RUN mkdir -p build \
&& cd build \
&& cmake .. \
&& make

ENV PATH $PATH:/home/Wunner/build/bin

WORKDIR /tmp
