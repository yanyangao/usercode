#!/bin/bash


export NJETS=$1;
export LUMI=$2

DATADIR=/Users/yanyan/CMS/SnT/WW/DYEst/LP2011/data/
METCUT=40
export FILLR=true

if [ ! $# -eq 2 ]; then
        echo "USAGE: ./doall.sh NJETS LUMI
        NJETS - number of jets 
        LUMI - lumi e.g. 1.143 stands for 1.143 /fb"
    exit 1
fi

#for MASS in 0 115 120 130 140 150 160 170 180 190 200 250 300; do
for  MASS in 190; do
#for MASS in 0; do
    for SAMPLE in dy; do  
	echo doing $MASS for sample $SAMPLE
	echo doing "root -l -q dycalc.C\+\($MASS,$NJETS,$METCUT,$LUMI,\"$DATADIR/\"\);"
	root -b -l -q dycalc.C\+\($MASS,$NJETS,$METCUT,$LUMI,\"$DATADIR/\",true,${FILLR}\);
	echo doing "root -l -q draw.C\($MASS,$NJETS,$LUMI,\"$SAMPLE\"\);"
	root -b -l -q draw.C\($MASS,$NJETS,$LUMI,\"$SAMPLE\"\);
    done
done
