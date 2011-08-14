ds#!/bin/bash

export NJETS=$1;

LUMI=1545

if [ ! $# -eq 1 ]; then
        echo "USAGE: ./print.sh NJETS
        NJETS - number of jets "
    exit 1
fi

echo "| selection | *R* |*N(in) (data)*| N(in) data/MC|* N(out)-data* | N(out) MC | N(out) data/MC|"

#for MASS in 0 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600; do
for MASS in 0 120 130 140 150 160 170 180 190 200 250 300; do
#for MASS in 0 ; do
    FILE=dyest_${NJETS}Jet_Metcut40_mH${MASS}_${LUMI}pb.txt
    SELECTION=$MASS
    #`echo $FILE | sed 's/dyest_\(.*\).txt/\1/'`
    R=`grep 'R(EE+MM)' $FILE | awk '{print $3""$4""$5""$6""$7}'`
    DATADRIVEN=`grep -C 1 'data-driven estimate:' $FILE | tail -n 1 | awk '{print $12""$13""$14""$15""$16}'`
    ZDATA=`grep -C 1 'Number of events inside Z window ' $FILE | tail -n 1 | awk '{print $1"/"$2"/"$3"/"$4}'`
    ZDATASUB=`grep -C 1 'OF/VZ subtracted ' $FILE | tail -n 1 | awk '{print $7""$8""$9}'`
    MC=`grep -C 1 'MC estimation in signal region' $FILE | tail -n 1 | awk '{print $7""$8""$9}'`
    OUTSCALE=`grep -C 1 'data/MC scale factor' $FILE | tail -n 1 | awk '{print $13""$14""$15""$16""$17}'`
    INSCALE=`grep -C 1 'data/MC scale factor from Z-window ' $FILE | tail -n 1 | awk '{print $8""$9""$10}'`
    #echo " \$m_{H}=$SELECTION\$ & \$ $ZDATASUB \$ & \$ $DATADRIVEN \$ & \$ $MC \$ \\\\ \hline"
    echo " $SELECTION & \$ $ZDATASUB \$ & \$ $R \$ & \$ $DATADRIVEN \$ & \$ $MC \$ \\\\"
done
