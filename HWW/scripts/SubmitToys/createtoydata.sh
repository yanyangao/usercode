#!/bin/bash
# scripts to create toy data based on S+B
# make sure the lands directory is correct

LANDS="../../../../LandS/test/lands.exe"


for CARDNAME in hwwof_0j_shape_8TeV  hwwof_1j_shape_8TeV xwwof_0j_shape_8TeV  xwwof_1j_shape_8TeV; do 
    $LANDS -d $CARDNAME.txt  -M Hybrid  -m 125 --minuitSTRATEGY 0 --bWriteToys 1 -n $CARDNAME --nToysForCLsb 10000 --nToysForCLb 1 --singlePoint 1 --seed 12344  -rMin 0 -rMax 5
done