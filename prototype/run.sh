#!/bin/bash
# This is the simple execution script that links the program to the c4.5 classifierby the means of file exchanges.
rm -r cawax.data stand.data walk.data lie.data
echo "[Bash] all old data files removed"
make  # build
./cawax $1 # run the program, final out is fed to c4.5 through the means of file exchange
c4.5 -f cawax
c4.5 -f stand
c4.5 -f walk
c4.5 -f lie


