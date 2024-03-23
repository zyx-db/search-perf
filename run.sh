#!/bin/bash
set -e

DATA=script-data.csv
IMAGE=perf.jpg
VENV=script-venv

# make exe
make bench

# set up env to analyze file
python3 -m venv $VENV
source $VENV/bin/activate
pip3 install -r ./analysis/requirements.txt

# run exe
./bench > $DATA

# analyze data
python3 ./analysis/analyze.py $DATA $IMAGE

# clean up everything but the image, including the exe
rm -rf $VENV $DATA bench
