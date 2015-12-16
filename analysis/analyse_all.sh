#!/bin/bash

FORCE=n
FILES=`ls ../data`

for i in $FILES; do
    RUN=${i#run}
    RUN=${RUN%.mid.gz}
    if [[ $FORCE == y ]] || [[ ! -e "run$RUN.root" ]]; then
        ./analyse.sh $RUN
    fi
done

