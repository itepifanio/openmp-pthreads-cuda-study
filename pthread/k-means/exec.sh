#!/bin/bash

rm -f kmeans.log && make clean && make && ./bin/exec "$@"
