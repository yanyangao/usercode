#!/bin/bash
#
# this code uses the  gcRunNormTest.sh to submit jobs
# for test do ./submitall.sh xww 0 1 1
# 

export ANA=$1
export NJET=$2
export NJOBS=$3
export NTOYS=$4

export MH=125

if [ ! $# -eq 4 ]; then
    echo " USAGE: ./submitall.sh ANA NJET NJOBS NTOYS
    ANA   - analysis, choose from hww and xww
    NET   - number of jets
    NJOBS - The number of jobs
    NTOYS - The number of pseudoexperiments per job"
    exit 1
fi


for JOB in $(seq 0 1 $(($NJOBS-1))); do
    ./gcRunNormTest.sh  $MH $(($(($JOB*$NTOYS))+1)) $(($(($JOB*$NTOYS))+$NTOYS)) $ANA $NJET 

done
