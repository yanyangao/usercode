#!/bin/bash


export NJETS=$1;
export LUMI=$2

#DATADIR=/smurf/data/Run2011_Spring11_SmurfV6_42X/mitf-alljets/
DATADIR=../SmurfV742Xdata
METCUT=37
export FILLR=true

if [ ! $# -eq 2 ]; then
        echo "USAGE: ./doall.sh NJETS LUMI
        NJETS - number of jets 
        LUMI - lumi e.g. 2121 in unit of pb"
    exit 1
fi


# write the output plots into a tex version
if [ -f R${NJETS}j_${LUMI}pb.tex ]; then
        rm -f R${NJETS}j_${LUMI}pb.tex
fi

echo writing document R${NJETS}j.tex

echo "\documentclass{article}" > R${NJETS}j_${LUMI}pb.tex
echo "\usepackage{times}" >> R${NJETS}j_${LUMI}pb.tex
echo "\usepackage{epsfig}" >> R${NJETS}j_${LUMI}pb.tex
echo "\begin{document}" >> R${NJETS}j_${LUMI}pb.tex


#for MASS in 0 115 120 130 140 150 160 170 180 190 200 250 300; do
for MASS in 0; do
    
    echo "\begin{figure}[htb]" >> R${NJETS}j_${LUMI}pb.tex
    echo "\begin{center}" >> R${NJETS}j_${LUMI}pb.tex
    echo "\begin{tabular}{ccc}" >> R${NJETS}j_${LUMI}pb.tex

    for SAMPLE in dy; do  
	echo doing $MASS for sample $SAMPLE
	echo doing "root -l -q dycalc.C\+\($MASS,$NJETS,$METCUT,$LUMI,\"$DATADIR/\"\);"
	root -b -l -q dycalc.C\+\($MASS,$NJETS,$METCUT,$LUMI,\"$DATADIR/\",true,${FILLR}\);
	echo doing "root -l -q draw.C\($MASS,$NJETS,$LUMI,\"$SAMPLE\"\);"
	root -b -l -q draw.C++\($MASS,$NJETS,$LUMI,\"$SAMPLE\"\);
        echo "\epsfig{figure=Routin_ee_${NJETS}Jet_mH${MASS}_${LUMI}pb_${SAMPLE}.eps, width=1.6in}" >> R${NJETS}j_${LUMI}pb.tex
        echo "\epsfig{figure=Routin_mm_${NJETS}Jet_mH${MASS}_${LUMI}pb_${SAMPLE}.eps, width=1.6in}" >> R${NJETS}j_${LUMI}pb.tex
        echo "\epsfig{figure=Routin_${NJETS}Jet_mH${MASS}_${LUMI}pb_${SAMPLE}.eps, width=1.6in}" >> R${NJETS}j_${LUMI}pb.tex
    done

    echo "\end{tabular}" >> R${NJETS}j_${LUMI}pb.tex
    echo "\caption{ Routin dependence on the met for ee (left) mm (middle) and ee/mm for mH=${MASS} GeV.}" >> R${NJETS}j_${LUMI}pb.tex
    echo "\end{center}" >> R${NJETS}j_${LUMI}pb.tex
    echo "\end{figure}" >> R${NJETS}j_${LUMI}pb.tex
done

echo "\end{document}" >> R${NJETS}j_${LUMI}pb.tex

#latex R${NJETS}j_${LUMI}pb.tex
#dvipdf R${NJETS}j_${LUMI}pb.dvi

