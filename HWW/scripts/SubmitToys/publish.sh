#!/bin/bash

# this code publish the plots into a webpage


WEBDIR=$1 
PLOTDIR=$2

if [ ! $# -eq 2 ]; then
    echo " USAGE: ./publish.sh webdir plotdir
    webdir  - the directory where you want to publish the plots
    plotdir - the directory where you keep the norm*.png"
    exit 1
fi



LUMI=19.5
for ANA in hww xww; do
    for NJET in 0; do 
	export outputdir=$WEBDIR/${ANA}${NJET}j/
	mkdir -p $outputdir
	cp $PLOTDIR/norm_inj125_${NJET}j_125_sfit_*${ANA}*.png toys_${ANA}of_${NJET}j_injmH125.png $outputdir
	./publishplot.pl $outputdir --t "${ANA} analysis in ${NJET} jet bin for ${LUMI}/fb"
    done
done
