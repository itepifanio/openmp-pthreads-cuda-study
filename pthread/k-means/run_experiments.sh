#!/bin/bash

mkdir -p data
if [ ! -f rice.zip ]; then
    echo "Downloading rice.zip..."
    curl https://archive.ics.uci.edu/static/public/545/rice+cammeo+and+osmancik.zip -o rice.zip
    unzip -o rice.zip -d data/rice
else
    echo "rice.zip already exists, skipping download."
fi

if [ ! -f htru2.zip ]; then
    echo "Downloading htru2.zip..."
    curl https://archive.ics.uci.edu/static/public/372/htru2.zip -o htru2.zip
    unzip -o htru2.zip -d data/htru2
else
    echo "htru2.zip already exists, skipping download."
fi

if [ ! -f miniboone.zip ]; then
    echo "Downloading miniboone.zip..."
    curl https://archive.ics.uci.edu/static/public/199/miniboone+particle+identification.zip -o miniboone.zip
    unzip -o miniboone.zip -d data/miniboone
else
    echo "miniboone.zip already exists, skipping download."
fi

if [ ! -f wesad.zip ]; then
    echo "Downloading wesad.zip..."
    curl https://uni-siegen.sciebo.de/s/HGdUkoNlW1Ub0Gx/download -o wesad.zip
    unzip -o wesad.zip -d data/wesad
else
    echo "wesad.zip already exists, skipping download."
fi

# remove old data
rm -f ./*.gif && rm -f experiments/*.csv && rm -f kmeans.log

# remove old executables and recompile
make clean && make

# run the program with debug false
./bin/exec 30 all "$@" 0
