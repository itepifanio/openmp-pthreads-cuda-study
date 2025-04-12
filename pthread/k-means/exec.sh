#!/bin/bash

# remove old data
rm ./*.gif && rm experiments/*.csv && rm -f kmeans.log

# remove old executables and recompile
make clean && make

# run the program
./bin/exec "$@" && python visualize.py
