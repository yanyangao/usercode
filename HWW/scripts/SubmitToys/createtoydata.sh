#!/bin/bash
# scripts to create toy data based on S+B
# make sure the lands directory is correct

LANDS="../../../../LandS/test/lands.exe"
CARD=$1

if [ ! $# -eq 1 ]; then
    echo " USAGE: ./createtoydata.sh cardname
    cardname - name of the card, e.g.hwwof_0j_shape_8TeV, ! without .txt !"
    exit 1
fi


$LANDS -d $CARD.txt  -M Hybrid  -m 125 --minuitSTRATEGY 0 --bWriteToys 1 -n $CARDNAME --nToysForCLsb 1000 --nToysForCLb 1 --singlePoint 1 --seed 12344  -rMin 0 -rMax 5
