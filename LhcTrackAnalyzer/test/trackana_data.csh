#! /bin/tcsh 

####################################################
## script to submit jobs for collision data
## argv[1] == 1 
# prepare cfg files
# sequence to choose from:
# only_analyze, re_tracking, re_reconstruction (Giovanni's sequence), refitting
## argv[1] == 2
# submit cmsRun jobs
## argv[1] == 3 
# plot Tracking parameters for each sample
# run the trackingPlot.C 
# publish plots in given webpage
## argv[1] == 4 
# Compare two samples
# run compSample.C macro 
## argv[1] == 5
# Compare different cuts on the same sample
# run compCut.C macro 
####################################################

### Configuration ############################################################
set Release=$CMSSW_VERSION
#set runstring="Run124024"
#set samples=("MB_{Dec9thReReco}")
set samples=("MinBias900GeV_BSCNOBEAMHALO_Dec19thSkim_v1")
set GlobalTag=("GR09_R_V5")
set Events=-1
set cfg="trackana_data_cfg.py"

###==== Set work directory
#set workdir="/store/disk02/yanyangao/LhcTrackAnalyzer/$Release" # at cms-tas03
set workdir="/uscms_data/d1/ygao/LhcTrackAnalyzer/$Release" # uscms
set cfgfiledir="$workdir/cfgfiles"
set outputdir="$workdir/ntuple"
set logdir="$workdir/log/"
set macrodir="$workdir/macro/"
set pngdir="$workdir/pngfiles"
set epsdir="$workdir/epsfiles"

mkdir -p $workdir
mkdir -p $cfgfiledir
mkdir -p $outputdir
mkdir -p $logdir
mkdir -p $macrodir
mkdir -p $pngdir
mkdir -p $epsdir

###==== Set publish dir
set ifpublish="true"

if ($ifpublish == "true") then 
  set publishdir="/afs/fnal.gov/files/home/room2/ygao/public_html/CMS/Tracking/LhcTrackAnalyzer/$Release" # At fnal
  #set publishdir="/afs/cern.ch/user/y/yygao/www/LhcTrackAnalyzer/$Release" # at cern
  mkdir -p $publishdir
endif


###====Set the track collecitons and vertex collections
set sequence="only_analyze"
set TrackCollection="generalTracks"
set secTrackCollection="ctfPixelLess"
set vertexCollection="offlinePrimaryVertices"
set pixelVertexCollection="pixelVertices"


##############################################################################
if ($1 == 1) then

echo "you chose option 1: prepare cfg files "  

set runSecTrackColl="True"
foreach sample($samples)
    echo "for " $sample
   # set different cuts for different samples
    switch ($sample)

    case "Run122294"
    set dataset="BeamCommissioning09_ExpressPhysics_Run122294"
    breaksw

    default:
    set dataset=$sample
   endsw

   if(! -e ../python/${dataset}_cff.py) then 
      echo "Missing "$sample"_cff.py, skipping this sample..."
       continue  
    endif
    cat $cfg | sed \
      -e s@SEQUENCE@$sequence@g \
      -e s@DATASET@$dataset@g \
      -e s@NEVENT@$Events@g \
      -e s@GLOBALTAG@$GlobalTag@g \
      -e s@SAMPLE@$sample@g \
      -e s@RELEASE@$Release@g \
      -e s@TRACKCOLLECTION@$TrackCollection@g \
      -e s@RUNSECTRACKCOLL@$runSecTrackColl@g \
      -e s@SECTRKCOLLECTION@$secTrackCollection@g \
      -e s@OUTPUTDIR@$outputdir@g \
      -e s@PVTXCOLLECTION@$vertexCollection@g \
      -e s@PIXELVERTEXCOLLECTION@$pixelVertexCollection@g \
      > ! $cfgfiledir/{$sample}_{$GlobalTag}_{$sequence}_cfg.py
end


##############################################################################
else if ($1 == 2) then
echo "you chose option 2:  submit cmsRun jobs"

eval `scramv1 run -csh`

foreach sample($samples)

  echo "for " $sample

  cmsRun $cfgfiledir/{$sample}_{$GlobalTag}_{$sequence}_cfg.py >& ! $logdir/{$sample}_{$GlobalTag}_{$sequence}.log < /dev/zero &

end
##############################################################################
##############################################################################
##############################################################################
else if ($1 == 3) then

 echo "you chose option 3:  run trackingPlot.C"

 if(! -e trackingPlotFirstCollision.C) then                                                    
      echo "Missing trackingPlotFirstCollision.C macro, skipping this sample..."
      continue
 endif       

 set runSecTrackColl="true"
 set normScale="0" # 0: no normailziation; 1: normalize by Entries; 2: normalize by Histogram Integral
 ## Specify the cuts
 #set cutstring="TrkInGoodPvtxCutBG"
  set cutstring="GoodPvtxCutnTracks"

 switch ($cutstring)
   case "nocut"
      set evtselection="1" 
      set trkselection="1" 
    breaksw

    case "GoodPvtx"
      set evtselection="hasGoodPvtx==1" 
      set trkselection="1" 
    breaksw

    case "HighPurityGoodPvtx"
      set evtselection="hasGoodPvtx==1" 
      set trkselection="ctf_isHighPurity==1"
    breaksw

  case "HighPurityGoodPvtx"
    set evtselection="hasGoodPvtx==1"
    set trkselection="ctf_isHighPurity==1"
  breaksw

                 
 case "GoodPvtxCutnTracks" 
  set evtselection="ctf_n<100"
  set trkselection="1"
 breaksw    

 case "GoodPvtxCutfHighPurity"
  set evtselection="ctf_fHighPurity>0.2"
  set trkselection="1"
  breaksw

case "TrkInGoodPvtxCutBG"
  set evtselection="ctf_fHighPurity>0.2"
  set trkselection="ctf_trkWeightpvtx>0.5"
  breaksw


 default:

      set evtselection="1" 
      set trkselection="1" 
   endsw


 foreach sample($samples)

   echo "run trackingPlot root macro for " $sample 
   
   mkdir -p $pngdir/$sample/$cutstring
   mkdir -p $epsdir/$sample/$cutstring

   rm -f $pngdir/$sample/$cutstring/*.png
   rm -f $epsdir/$sample/$cutstring/*.eps
  
   cat trackingPlotFirstCollision.C  | sed \
      -e s@SAMPLE@$sample@g \
      -e s@GLOBALTAG@$GlobalTag@g \
      -e s@SEQUENCE@$sequence@g \
      -e s@CUTSTRING@$cutstring@g \
      -e s@EVTSELECTION@$evtselection@g \
      -e s@TRKSELECTION@$trkselection@g \
      -e s@ROOTDIR@$outputdir@g \
      -e s@PNGDIR@$pngdir@g \
      -e s@EPSDIR@$epsdir@g \
      -e s@RUNSECTRACKCOLL@$runSecTrackColl@g \
      -e s@TRACKCOLLECTION@$TrackCollection@g \
      -e s@SECTRKCOLLECTION@$secTrackCollection@g \
      -e s@NORMSCALE@$normScale@g \
      > ! $macrodir/{$sample}.C

   root -b -q -l $macrodir/{$sample}.C > ! $logdir/macro.$sample.$cutstring.log

   if ($ifpublish == "true") then 
 
    mkdir -p $publishdir/$sample/$cutstring

    if(! -e makeTrackHtml.py) then
      echo "Missing webpage making script makeTrackHtml.py, skipping this sample..."
       continue
    endif         
   
    echo "publish trackingplot in the web for " $sample
    rm -f $publishdir/$sample/$cutstring/*.png

    cp $pngdir/$sample/$cutstring/*.png $publishdir/$sample/$cutstring
 
    cat makeTrackHtml.py | sed \
      -e s@SAMPLE@$sample@g \
      -e s@GLOBALTAG@$GlobalTag@g \
      -e s@RELEASE@$Release@g \
      > ! $publishdir/$sample/$cutstring/makeHtml.py

    chmod u+x $publishdir/$sample/$cutstring/makeHtml.py 

    $publishdir/$sample/$cutstring/makeHtml.py $publishdir/$sample/$cutstring

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

 set comptrk="compdatamc"

set newfile="work/ntuple/MinBias_START3X_V26B_preprod_only_analyze.root"
set newlabel="MB7TeV_V26B358p2"

set reffile="/uscms_data/d2/ygao/LhcTrackAnalyzer/CMSSW_3_5_7/ntuple/MinBias_Spring10_START3X_V26A_357ReReco_only_analyze.root"
set reflabel="V26A357ReReco"


 set ctfOrSecTrk="1" #1: ctf; 2: sectrk
 set normScale="3" #0: do nothing; 1: normalizeByEntries; 2: normalizeByIntegral  3: normalizationByEvents  
 
## Specify the cuts
#set cutstring="nocut"
set cutstring="GoodPvtxHighPurity"

switch ($cutstring)
   case "nocut"
      set evtselection="1" 
      set trkselection="1" 
    breaksw

    case "GoodPvtxHighPurity"
      set evtselection="ctf_fHighPurity>0.2"
      set trkselection="ctf_isHighPurity==1"
    breaksw
  
   case "CutfHighPurity"
   set evtselection="ctf_fHighPurity>0.2"
   set trkselection="1"
   breaksw

 case "GoodPvtxCutfHPnLayers"
    set evtselection="ctf_fHighPurity>0.2"
    set trkselection="ctf_nLayers>4"
    breaksw

  case "GoodPvtxCutfHighPurity"
  set evtselection="ctf_fHighPurity>0.2"
   set trkselection="1"
   breaksw

  case "GoodPvtxCutnTracks"
  set evtselection="ctf_n<100"
  set trkselection="1"
    breaksw
  case "HighPurityGoodPvtx"
  set evtselection="ctf_fHighPurity>0.2"
  set trkselection="ctf_isHighPurity==1"
    breaksw               

          default:
      set evtselection="1" 
      set trkselection="1" 
   endsw

 if($normScale == 3) then
 set comppngdir="$pngdir/comp_{$newlabel}_{$reflabel}/NormByEvts/$cutstring/"
 set compepsdir="$epsdir/comp_{$newlabel}_{$reflabel}/NormByEvts/$cutstring/"
 endif 

 if($normScale == 1) then
 set comppngdir="$pngdir/comp_{$newlabel}_{$reflabel}/NormByEntries/$cutstring/"
 set compepsdir="$epsdir/comp_{$newlabel}_{$reflabel}/NormByEntries/$cutstring/"
 endif 



 mkdir -p $comppngdir
 mkdir -p $compepsdir

 cat compSample.C | sed \
      -e s@COMPTRK@$comptrk@g \
      -e s@NEWFILE@$newfile@g \
      -e s@NEWLABEL@$newlabel@g \
      -e s@REFFILE@$reffile@g \
      -e s@REFLABEL@$reflabel@g \
      -e s@PNGDIR@$comppngdir@g \
      -e s@EPSDIR@$compepsdir@g \
      -e s@DOCFTSECTRK@$ctfOrSecTrk@g \
      -e s@NORMSCALE@$normScale@g \
      -e s@CUTSTRING@$cutstring@g \
      -e s@EVTSELECTION@$evtselection@g \
      -e s@TRKSELECTION@$trkselection@g \
      > ! $macrodir/$comptrk.C

 root -b -q -l $macrodir/$comptrk.C > ! $logdir/macro.$comptrk.log

 
 if ($ifpublish == "true") then  

    if(! -e makeCompTrackHtml.py) then
      echo "Missing webpage making script makeTrackHtml.py, skipping this sample..."
       continue
    endif         
    
    if($normScale == 1) then
    set comppublishdir="$publishdir/comp_{$newlabel}_{$reflabel}/NormByEntries/$cutstring/"
    endif

   if($normScale == 3) then
    set comppublishdir="$publishdir/comp_{$newlabel}_{$reflabel}/NormByEvts/$cutstring/"
    endif

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




##############################################################################
else if ($1 == 5) then

 echo "you chose option 5:  run compCut.C"

 if(! -e compCut.C) then
      echo "Missing compCut.C macro, skipping this sample..."
      continue
 endif       

foreach sample($samples)

 set file="$outputdir/{$sample}_{$GlobalTag}_{$sequence}.root"
 echo $file
    
 set newlabel="Run124120"
 set reflabel="Run124275"

 set ctfOrSecTrk="1" #1: ctf; 2: sectrk
 set normScale="2" #0: do nothing; 1: normalizeByEntries; 2: normalizeByIntegral    

 # cut1
 set evtselection1="glob_runno==124120"
 set trkselection1="1"
 # cut2
 set evtselection2="glob_runno==124275"
 set trkselection2="1"

 set comppngdir="$pngdir/comp_{$newlabel}_{$reflabel}/"
 set compepsdir="$epsdir/comp_{$newlabel}_{$reflabel}/"

 mkdir -p $comppngdir
 mkdir -p $compepsdir

 cat compCut.C | sed \
      -e s@FILE@$file@g \
      -e s@NEWLABEL@$newlabel@g \
      -e s@REFLABEL@$reflabel@g \
      -e s@PNGDIR@$comppngdir@g \
      -e s@EPSDIR@$compepsdir@g \
      -e s@DOCFTSECTRK@$ctfOrSecTrk@g \
      -e s@NORMSCALE@$normScale@g \
      -e s@EVTSELECTION1@$evtselection1@g \
      -e s@TRKSELECTION1@$trkselection1@g \
      -e s@EVTSELECTION2@$evtselection2@g \
      -e s@TRKSELECTION2@$trkselection2@g \
      > ! $macrodir/compCut.C

 root -b -q -l $macrodir/compCut.C > ! $logdir/macro.compCut.log

 
 if ($ifpublish == "true") then  

    if(! -e makeCompTrackHtml.py) then
      echo "Missing webpage making script makeTrackHtml.py, skipping this sample..."
       continue
    endif         
 
   set comppublishdir="$publishdir/$sample/comp_{$newlabel}_{$reflabel}/"
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

end
endif
