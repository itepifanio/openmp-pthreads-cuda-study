#!/bin/bash

# remove old data
rm -f ./*.gif && rm -f experiments/*.csv && rm -f kmeans.log

# remove old executables and recompile
make clean && make

# run the program with debug false
./bin/exec 30 all "$@" 0
