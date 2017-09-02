FROM ubuntu:xenial

RUN apt-get update && apt-get install -y \
build-essential \
cmake \
libboost-regex-dev \
wget

COPY ./entrypoint.sh /home
ENTRYPOINT ["/home/entrypoint.sh"]
