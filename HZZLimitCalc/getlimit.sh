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
#for MASS in 250; do
    CARDEE=`ls ${CARDDIR}/${MASS}/*.txt | grep ee`
    CARDMM=`ls ${CARDDIR}/${MASS}/*.txt | grep mm`
    echo $CARDEE $CARDMM
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
