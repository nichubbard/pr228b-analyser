#!/bin/sh -e

cd ../build
make
cp analyzer ../analyzer
cd ../
cp src/RawData.h src/SiliconData.h src/GammaData.h .

COPY_ROOT=y

for r in "$@"
do
    RUN=$r
    RUN=`printf "%05d" $RUN`

    MIDAS_FILE="../data/run$RUN.mid.gz"
    ROOT_FILE="./analysis/run$RUN.root"

    if [ ! -e $MIDAS_FILE ]; then
        if [ $COPY_ROOT = y ]; then
            scp nic@stkernfys.phys.au.dk:/nfs/nic/pr228b/data/run$RUN.mid.gz $MIDAS_FILE
        else
            echo "Missing run file for run $r"
            exit 1
        fi
    fi

    ./analyzer -i $MIDAS_FILE
    mv output.root $ROOT_FILE
done
notify-send "Sort completed"

