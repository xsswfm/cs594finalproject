#!/bin/bash

DISTINCT=50
ALL=2000
((FREQ= ALL / DISTINCT))
HASH=200
THREAD=4

rm -r ./data

mkdir ./data/
mkdir ./data/pbfResult

./generate.exe --distinct $DISTINCT --total $ALL

./pbfreg.exe -n $ALL -k $HASH -f $FREQ --pbfOutput ./data/pbfResult/reg_result.txt

./pbfomp.exe -n $ALL -k $HASH -f $FREQ -t $THREAD --pbfOutput ./data/pbfResult/omp_result.txt

#./pbfgpu.exe -n $ALL -k $HASH -f $FREQ --pbfOutput ./data/pbfResult/gpu_result.txt


