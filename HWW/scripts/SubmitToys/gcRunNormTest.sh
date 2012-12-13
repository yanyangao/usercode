#!/bin/bash

# this code submits the bias studies to the lsf queue
#--------Test example, submitting 1 job to the queue
# ./gcRunNormTest.sh 125 0 0 hww 0
#--------Realistic example, submit 10 jobs into the queue, which 
#     analyses 100 toys 
# for j in {0,100,200,300,400,500,600,700,800,900}; do ./gcRunNormTest.sh 125 $(($j+1)) $(($j+100)) hww 0; done


#this is def or 125
INJ=$1
#these specify the range of toy to process
MIN=$2
MAX=$3
ANA=$4
NJETS=$5


OUTPUTDIR=/afs/cern.ch/user/y/yygao/scratch0/ana_PostHCP_2D_19fb/
mkdir -p $OUTPUTDIR/logsNorm/


if [ ! $# -eq 5 ]; then
    echo " USAGE: ./gcRunNormTest.sh inj min max ana njets
    inj - choose from def and 125 (set to 125 for signal injection 
    min - range of the toys to process 
    max - range of the toys to process 
    ana - analysis, choose from hww and xww, xww is for spin 2
    njet - number of jets "
    exit 1
fi

# for the batch submission
WRAPPER="process_job.sh"
QUEUE="1nd"
WORKDIR=`pwd`
# write the wrapper
cat > ${WRAPPER} << EOF
#!/bin/bash
INJ=\$1
MIN=\$2
MAX=\$3
MASS=\$4
cd /tmp/yygao/
scramv1 p CMSSW CMSSW_5_3_3
cd CMSSW_5_3_3/src
eval \`scramv1 ru -sh\`
addpkg HiggsAnalysis/CombinedLimit V02-02-03 
scramv1 b
perl -p -i -e "s/exp\_/exp\_final\_/g" HiggsAnalysis/CombinedLimit/test/mlfitNormsToText.py
cp $WORKDIR/getPseudoData.C .
root -b -q getPseudoData.C\(\${MASS},\"8TeV\",\"shape\",$NJETS,\"of\",\"${ANA}\",\${MIN},\${MAX}\)
echo 'processing mass='\${MASS} analysis ${ANA} $NJET jet>> /tmp/yygao/CMSSW_5_3_3/src/log_\${INJ}_\${MASS}_\${MIN}_\${MAX}_$ANA_${NJETS}j_summary.log
for NEXP in \$(seq \${MIN} \${MAX})
  do
  echo 'toy='\${NEXP} 
  DIRPATH=testcards/cards_\${INJ}_N\${NEXP}/\${MASS}/
  cd \${DIRPATH}
  combine -M MaxLikelihoodFit --saveNorm ${ANA}of_${NJETS}j_shape_8TeV.txt -m \${MASS} #>& /dev/null
  python ../../../HiggsAnalysis/CombinedLimit/test/mlfitNormsToText.py mlfit.root >> /tmp/yygao/CMSSW_5_3_3/src/logNorm_\${INJ}_\${MASS}_${ANA}of_${NJETS}j_shape_8TeV_\${MIN}_\${MAX}.log
  python ../../../HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a mlfit.root >> /tmp/yygao/CMSSW_5_3_3/src/logNuis_\${INJ}_\${MASS}_${ANA}of_${NJETS}j_shape_8TeV_\${MIN}_\${MAX}.log
  rm mlfit.root higgsCombineTest.MaxLikelihoodFit.mH125.root
  cd /tmp/yygao/CMSSW_5_3_3/src
done
mkdir -p $OUTPUTDIR/logsNorm/\${MASS}/
# clear up old files
rm -f $OUTPUTDIR/logsNorm/\${MASS}/logNorm_\${INJ}_\${MASS}_${ANA}of_${NJETS}j_shape_8TeV_\${MIN}_\${MAX}.log
rm -f $OUTPUTDIR/logsNorm/\${MASS}/logNuis_\${INJ}_\${MASS}_${ANA}of_${NJETS}j_shape_8TeV_\${MIN}_\${MAX}.log
rm -f $OUTPUTDIR/logsNorm/\${MASS}/log_\${INJ}_\${MASS}_\${MIN}_\${MAX}_$ANA_${NJETS}j_summary.log
cp /tmp/yygao/CMSSW_5_3_3/src/log* $OUTPUTDIR/logsNorm/\${MASS}/
cd /tmp/yygao/
rm -rf CMSSW_5_3_3
EOF

# submit the jobs
echo 'injection: '$INJ' - processing toys:'$MIN'-'$MAX
for MASS in 125
  do
  chmod u+x ${WRAPPER}
  rm -f joblog_${INJ}_${MASS}_${MIN}_${MAX}_${ANA}_${NJETS}j.log
  bsub -q ${QUEUE} -o joblog_${INJ}_${MASS}_${MIN}_${MAX}_${ANA}_${NJETS}j.log "${WRAPPER} ${INJ} ${MIN} ${MAX} ${MASS} ${NJETS}"
done


