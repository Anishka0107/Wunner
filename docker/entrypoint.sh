#! /bin/bash

mkdir -p /tmp/build
cd /tmp/build

cmake -DBUILD_STATIC=1 ..
make
