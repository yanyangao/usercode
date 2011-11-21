#!/bin/bash

export CARDDIR=$1;

if [ ! $# -eq 1 ]; then
	echo "USAGE: ./getlimit.sh CARDDIR
	carddir - the input directory with the cards "
	exit 1
fi

rm -f ${CARDDIR}/limits.log
rm -f ${CARDDIR}/limits.txt

echo "Mass  Median Expected 68% C.L. band 95%% C.L. band" >> ${CARDDIR}/limits.log

for MASS in 250 300 350 400 500 600; do 
    CARDEE=`ls ${CARDDIR}/${MASS}/*.txt | grep ee`
    CARDMM=`ls ${CARDDIR}/${MASS}/*.txt | grep mm`
    combineCards.py mm=${CARDMM} ee=${CARDEE} > ${CARDDIR}/card_${MASS}.txt
    combine -d ${CARDDIR}/card_${MASS}.txt -M Asymptotic > ${CARDDIR}/${MASS}_result.log
    OBSERVED=`grep 'Observed Limit' ${CARDDIR}/${MASS}_result.log | awk '{printf "%4.1f\n", $5}'`
    MEDIAN=`grep 'Expected 50.0%' ${CARDDIR}/${MASS}_result.log | awk '{printf "%4.2f\n", $5}'`
    MINUS1SIG=`grep 'Expected 16.0%' ${CARDDIR}/${MASS}_result.log | awk '{printf "%4.2f\n", $5}'`
    PLUS1SIG=`grep 'Expected 84.0%' ${CARDDIR}/${MASS}_result.log | awk '{printf "%4.2f\n", $5}'`
    MINUS2SIG=`grep 'Expected  2.5%' ${CARDDIR}/${MASS}_result.log | awk '{printf "%4.2f\n", $5}'`
    PLUS2SIG=`grep 'Expected 97.5%' ${CARDDIR}/${MASS}_result.log | awk '{printf "%4.2f\n", $5}'`
    echo "$MASS $OBSERVED $MEDIAN [${MINUS1SIG}, ${PLUS1SIG}] [${MINUS2SIG}, ${PLUS2SIG}]"
    echo "$MASS & $OBSERVED & $MEDIAN & [${MINUS1SIG}, ${PLUS1SIG}] & [${MINUS2SIG}, ${PLUS2SIG}] \\\\" >> ${CARDDIR}/limits.log
    echo "$MASS $OBSERVED ${MINUS2SIG} ${MINUS1SIG} $MEDIAN ${PLUS1SIG} ${PLUS2SIG}" >> ${CARDDIR}/limits.txt
done

# Plot the limits
# write the script to analyze the outputs
rm -f makePlots.cxx
echo "void makePlots(const char* file=\"${CARDDIR}/limits.txt\", const char* title=\"H #rightarrow ZZ #rightarrow 2l2#nu all jet bins\")" >> makePlots.cxx
echo "{" >> makePlots.cxx
echo "  gSystem->Load(\"lands.so\");" >> makePlots.cxx
echo "  gSystem->CompileMacro(\"PlotExpectedLimits.C\",\"k\");" >> makePlots.cxx
echo "  PlotExpectedLimits(file,title);" >> makePlots.cxx
echo "}" >> makePlots.cxx 

root -q -b -l makePlots.cxx
epstopdf limits.eps
mv limits.eps limits.pdf ${CARDDIR}/