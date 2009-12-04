# Auto generated configuration file
# usin
# Revision: 1.49
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v
# with command line options: promptCollisionReco -s RAW2DIGI,L1Reco,RECO,DQM,ALCA:SiStripCalZeroBias --datatier RECO --eventcontent RECO --conditions CRAFT09_R_V4::All --scenario pp --no_exec --data --magField AutoFromDBCurrent -n 100
# From http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/Configuration/GlobalRuns/python/rereco.py?revision=1.2&view=markup&sortby=date

import FWCore.ParameterSet.Config as cms

process = cms.Process('trackana')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/RawToDigi_Data_cff')
process.load('Configuration/StandardSequences/L1Reco_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('DQMOffline/Configuration/DQMOffline_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# Refitters
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# cut RecoTracks
process.load("UserCode.LhcTrackAnalyzer.cutRecoTracks_cfi")

# suppress report message every 1000 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.2 $'),
            annotation = cms.untracked.string('promptCollisionReco nevts:100'),
            name = cms.untracked.string('PyReleaseValidation')
        )

process.options = cms.untracked.PSet(
        Rethrow = cms.untracked.vstring('ProductNotFound'),
            wantSummary = cms.untracked.bool(False)
        )
# Input source
#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring(
#    'file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_GLOBAL/Run122314_BSCSkim_MinBiasPD_ReTracking.root'
#    )
#                            )

process.load("UserCode.LhcTrackAnalyzer.SAMPLE_cff")
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(NEVENT) )

# Other statements
process.GlobalTag.globaltag = 'GLOBALTAG::All'

## By Giovanni for first collision
## Skip events with HV off
process.fifthSeeds.ClusterCheckPSet.MaxNumberOfCosmicClusters   = 5000
## Seeding: increase the region
process.fifthSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 100
process.fifthSeeds.RegionFactoryPSet.RegionPSet.originRadius     =   5
## Seeding: add TOB3 to the list, allow unmatched hits
process.fifthlayerpairs.TOB.useSimpleRphiHitsCleaner = cms.bool(False)
process.fifthlayerpairs.TOB.rphiRecHits = cms.InputTag("fifthStripRecHits","rphiRecHitUnmatched")
process.fifthlayerpairs.TOB.stereoRecHits = cms.InputTag("fifthStripRecHits","stereoRecHitUnmatched")
process.fifthlayerpairs.layerList += [ 'TOB1+TOB3', 'TOB2+TOB3' ] #, 'TOB1+TOB4', 'TOB2+TOB4' ]
## Pattern recognition: lower the cut on the number of hits
process.fifthCkfTrajectoryFilter.filterPset.minimumNumberOfHits = 5
process.fifthCkfTrajectoryFilter.filterPset.maxLostHits = 4
process.fifthCkfTrajectoryFilter.filterPset.maxConsecLostHits = 2
process.fifthCkfInOutTrajectoryFilter.filterPset.minimumNumberOfHits = 3
process.fifthCkfInOutTrajectoryFilter.filterPset.maxLostHits = 4
process.fifthCkfInOutTrajectoryFilter.filterPset.maxConsecLostHits = 2
process.fifthCkfTrajectoryBuilder.minNrOfHitsForRebuild = 2
## Pattern recognition: enlarge a lot the search window, as the true momentum is very small while the tracking assumes p=5 GeV if B=0
process.Chi2MeasurementEstimator.MaxChi2 = 200
process.Chi2MeasurementEstimator.nSigma  = 5
## Fitter-smoother: lower the cut on the number of hits
process.fifthRKTrajectorySmoother.minHits = 4
process.fifthRKTrajectoryFitter.minHits = 4
process.fifthFittingSmootherWithOutlierRejection.MinNumberOfHits = 4
## Fitter-smoother: loosen outlier rejection
process.fifthFittingSmootherWithOutlierRejection.BreakTrajWith2ConsecutiveMissing = False
process.fifthFittingSmootherWithOutlierRejection.EstimateCut = 50
## Quality filter
process.tobtecStepLoose.minNumberLayers = 3
process.tobtecStepLoose.minNumber3DLayers = 0
process.tobtecStepLoose.maxNumberLostLayers = 4
process.tobtecStepLoose.dz_par1 = cms.vdouble(100.5, 4.0)
process.tobtecStepLoose.dz_par2 = cms.vdouble(100.5, 4.0)
process.tobtecStepLoose.d0_par1 = cms.vdouble(1.5, 4.0)
process.tobtecStepLoose.d0_par2 = cms.vdouble(1.5, 4.0)
process.tobtecStepLoose.chi2n_par = cms.double(10.0)
process.tobtecStepLoose.keepAllTracks = True
process.tobtecStepTight = process.tobtecStepLoose.clone(
    keepAllTracks = True,
    qualityBit = cms.string('tight'),
    src = cms.InputTag("tobtecStepLoose"),
    minNumberLayers = 5,
    minNumber3DLayers = 1
)
process.tobtecStep = process.tobtecStepLoose.clone(
    keepAllTracks = True,
    qualityBit = cms.string('highPurity'),
    src = cms.InputTag("tobtecStepTight"),
    minNumberLayers = 4,
    minNumber3DLayers = 2
)

# for HF proper reconstruction
process.hfreco.firstSample = 4
process.hfreco.samplesToAdd = 4


## Redo PrimaryVertex fitter
process.offlinePrimaryVertices.PVSelParameters.maxDistanceToBeam = 10
process.offlinePrimaryVertices.TkFilterParameters.maxNormalizedChi2 = 500
process.offlinePrimaryVertices.TkFilterParameters.minSiliconHits = 5
process.offlinePrimaryVertices.TkFilterParameters.maxD0Significance = 100
process.offlinePrimaryVertices.TkFilterParameters.minPixelHits = -1
process.offlinePrimaryVertices.TkClusParameters.zSeparation = 10

# Output definition
process.FEVT = cms.OutputModule("PoolOutputModule",
                                    splitLevel = cms.untracked.int32(0),
                                    outputCommands = process.RECOEventContent.outputCommands,
                                    fileName = cms.untracked.string('rerecoOutput.root'),
                                    dataset = cms.untracked.PSet(
            dataTier = cms.untracked.string('RECO'),
                    filterName = cms.untracked.string('')
                )
                                )


#======================================
# TrackTupleMaker
#======================================
process.load("UserCode.LhcTrackAnalyzer.LhcTrackAnalyzer_cff")
process.trackana = process.LhcTrackAnalyzer.clone()
process.trackana.Debug = False
process.trackana.ctfTrackCollectionTag = "TRACKCOLLECTION"
process.trackana.runSecTrackColl = RUNSECTRACKCOLL
process.trackana.secTrackCollectionTag = "SECTRKCOLLECTION"
process.trackana.OutputFileName = cms.string("OUTPUTDIR/SAMPLE_GLOBALTAG_SEQUENCE.root")
process.trackana.vertexCollection = "PVTXCOLLECTION"
process.trackana.pixelVertexCollectionTag = "PIXELVERTEXCOLLECTION"



# only_analyze
process.only_analyze = cms.Sequence(process.trackana)

# re_tracking
process.re_tracking = cms.Sequence(
    ( process.siPixelRecHits * process.siStripMatchedRecHits ) *
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )


# re_reconstruction: 
process.re_reco = cms.Sequence(
    ( process.siPixelRecHits * process.siStripMatchedRecHits ) *
    process.offlineBeamSpot *
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )

# re_fitting
process.refitting = cms.Sequence(
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )

### final path and endPath
process.p = cms.Path(process.SEQUENCE)
