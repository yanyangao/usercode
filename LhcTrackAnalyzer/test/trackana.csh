#! /bin/tcsh 

## argv[1] == 1 
# prepare cfg files

## argv[1] == 2
# submit cmsRun jobs

## argv[1] == 3 
# plot Tracking parameters for each sample
# run the trackingPlot.C 
# publish plots in given webpage

## argv[1] == 4 
# Compare two samples
# run compSample.C macro 

### Configuration ############################################################
set Release="$CMSSW_VERSION"
set samples=("MinBias_Summer09_START3X_22A_900GeV_352_v1")
set GlobalTag="START3X_V22A"
set Events=50000
set cfg="trackana_cfg.py"

# Set work directory
set workdir="/uscms_data/d2/ygao/LhcTrackAnalyzer/$Release"
set cfgfiledir="$workdir/cfgfiles"
set outputdir="$workdir/ntuple"
set logdir="$workdir/log/"
set macrodir="$workdir/macro/"
set pngdir="$workdir/pngfiles"
set epsdir="$workdir/epsfiles"
set ifpublish="true"
set publishdir = "/afs/fnal.gov/files/home/room2/ygao/public_html/CMS/Tracking/LhcTrackAnalyzer"
mkdir -p $workdir
mkdir -p $cfgfiledir
mkdir -p $outputdir
mkdir -p $logdir
mkdir -p $macrodir
mkdir -p $publishdir
mkdir -p $pngdir
mkdir -p $epsdir

##############################################################################
if ($1 == 1) then

echo "you chose option 1: prepare cfg files "  

set TrackCollection="generalTracks" #default TrackCollection
set runSecTrackColl="False"
set secTrackCollection="generalTracks" #secondTrackCollection   

foreach sample($samples)
    echo "for " $sample
   # set different cuts for different samples
    switch ($sample)
    case "ttbar":
        set dataset="TTbar_Summer09_MC_31X_V3"
    breaksw

    case "minbias":
    set dataset="MinBias_Summer09_MC_31X_V3"
    breaksw

    default:
    set dataset=$sample
   endsw

   if(! -e ../python/${dataset}_cff.py) then 
      echo "Missing "$sample"_cff.py, skipping this sample..."
       continue  
    endif
    cat $cfg | sed \
      -e s@DATASET@$dataset@g \
      -e s@NEVENT@$Events@g \
      -e s@GLOBALTAG@$GlobalTag@g \
      -e s@SAMPLE@$sample@g \
      -e s@RELEASE@$Release@g \
      -e s@TRACKCOLLECTION@$TrackCollection@g \
      -e s@RUNSECTRACKCOLL@$runSecTrackColl@g \
      -e s@SECTRKCOLLECTION@$secTrackCollection@g \
      -e s@OUTPUTDIR@$outputdir@g \
      > ! $cfgfiledir/{$sample}_{$GlobalTag}_cfg.py
end


##############################################################################
else if ($1 == 2) then
echo "you chose option 2:  submit cmsRun jobs"

eval `scramv1 run -csh`

foreach sample($samples)

  echo "for " $sample

  cmsRun $cfgfiledir/{$sample}_{$GlobalTag}_cfg.py >& ! $logdir/{$sample}_{$GlobalTag}.log < /dev/zero &

end
##############################################################################
##############################################################################
else if ($1 == 3) then

 echo "you chose option 3:  run trackingPlot.C"

 if(! -e trackingPlot.C) then                                                                                                       
      echo "Missing trackingPlot.C macro, skipping this sample..."
      continue
 endif       

set TrackCollection="generalTracks" #default TrackCollection  
set runSecTrackColl="false"
set secTrackCollection="generalTracks" #secondTrackCollection 

 foreach sample($samples)

   echo "run trackingPlot root macro for " $sample 

   mkdir -p $pngdir/$sample
   mkdir -p $epsdir/$sample

   cat trackingPlot.C | sed \
      -e s@SAMPLE@$sample@g \
      -e s@GLOBALTAG@$GlobalTag@g \
      -e s@RELEASE@$Release@g \
      -e s@ROOTDIR@$outputdir@g \
      -e s@PNGDIR@$pngdir@g \
      -e s@EPSDIR@$epsdir@g \
      -e s@RUNSECTRACKCOLL@$runSecTrackColl@g \
      -e s@TRACKCOLLECTION@$TrackCollection@g \
      -e s@SECTRKCOLLECTION@$secTrackCollection@g \
      > ! $macrodir/$sample.C

    root -b -q -l $macrodir/$sample.C > ! $logdir/macro.$sample.log

 if ($ifpublish == "true") then  

    if(! -e makeTrackHtml.py) then
      echo "Missing webpage making script makeTrackHtml.py, skipping this sample..."
       continue
    endif         
   
    echo "publish trackingplot in the web for " $sample
    rm -f $publishdir/$sample/*.png

    cp $pngdir/$sample/*.png $publishdir/$sample
 
    cat makeTrackHtml.py | sed \
      -e s@SAMPLE@$sample@g \
      -e s@GLOBALTAG@$GlobalTag@g \
      -e s@RELEASE@$Release@g \
      > ! $publishdir/$sample/makeHtml.py

    chmod u+x $publishdir/$sample/makeHtml.py 

    $publishdir/$sample/makeHtml.py $publishdir/$sample

  endif
end

##############################################################################


##############################################################################
else if ($1 == 4) then

 echo "you chose option 4:  run compSample.C"

 if(! -e compSample.C) then                                                                                                       
      echo "Missing compSample.C macro, skipping this sample..."
      continue
 endif       

 set comptrk="compttbarminbias"

 set newfile="/uscms_data/d2/ygao/LhcTrackAnalyzer/CMSSW_3_1_2/ntuple/ttbar_MC_31X_V3_CMSSW_3_1_2.root"
 set reffile="/uscms_data/d2/ygao/LhcTrackAnalyzer/CMSSW_3_1_2/ntuple/minbias_MC_31X_V3_CMSSW_3_1_2.root"

 set newlabel="ttbar"
 set reflabel="minbias"

 set comppngdir="/uscms_data/d2/ygao/LhcTrackAnalyzer/CMSSW_3_1_2/pngfiles/comp_{$newlabel}_{$reflabel}/"
 set compepsdir="/uscms_data/d2/ygao/LhcTrackAnalyzer/CMSSW_3_1_2/epsfiles/comp_{$newlabel}_{$reflabel}/"

 mkdir -p $comppngdir
 mkdir -p $compepsdir

 set ctfOrSecTrk="1" #1: ctf; 2: sectrk
 set normScale="1" #0: do nothing; 1: normalizeByEntries; 2: normalizeByIntegral    

 cat compSample.C | sed \
      -e s@COMPTRK@$comptrk@g \
      -e s@NEWFILE@$newfile@g \
      -e s@NEWLABEL@$newlabel@g \
      -e s@REFFILE@$reffile@g \
      -e s@REFLABEL@$reflabel@g \
      -e s@PNGDIR@$comppngdir@g \
      -e s@DOCFTSECTRK@$ctfOrSecTrk@g \
      -e s@NORMSCALE@$normScale@g \
      > ! $macrodir/$comptrk.C

 root -b -q -l $macrodir/$comptrk.C > ! $logdir/macro.$comptrk.log
 
 if ($ifpublish == "true") then  

    if(! -e makeCompTrackHtml.py) then
      echo "Missing webpage making script makeTrackHtml.py, skipping this sample..."
       continue
    endif         
 
   set comppublishdir="$publishdir/comp_{$newlabel}_{$reflabel}"
   mkdir -p $comppublishdir

   rm -f $comppublishdir/*.png   

   cp $comppngdir/*.png $comppublishdir

   cat  makeCompTrackHtml.py | sed \
      -e s@NEWLABEL@$newlabel@g \
      -e s@REFLABEL@$reflabel@g \
      > ! $comppublishdir/makeHtml.py

   chmod u+x $comppublishdir/makeHtml.py 

   $comppublishdir/makeHtml.py $comppublishdir
 endif


endif
