#!/bin/sh

RUNS=`root -b -q runlist.C 2>/dev/null | tail -n 1`

./analyse.sh $RUNS

