import FWCore.ParameterSet.Config as cms

process = cms.Process('trackana')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/L1Reco_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('DQMOffline/Configuration/DQMOffline_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# Refitters
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")

# suppress report message every 1000 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.1 $'),
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

process.load("INPUT_FILE")
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(NEVENT) )

# Other statements
process.GlobalTag.globaltag = 'GLOBALTAG::All'

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
process.trackana.ctfTrackCollectionTag = "generalTracks"
process.trackana.runSecTrackColl = False
process.trackana.secTrackCollectionTag = "generalTracks"
process.trackana.OutputFileName = cms.string("OUTFILE")
process.trackana.vertexCollection = "offlinePrimaryVertices"
process.trackana.pixelVertexCollectionTag = "pixelVertices"

#======================================
# Trigger Filter
#======================================
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
process.bit40 = hltLevel1GTSeed.clone(
        L1TechTriggerSeeding = cms.bool(True),
            L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')
            )


# only_analyze
process.only_analyze = cms.Sequence(process.bit40*process.trackana)   

# re_tracking
process.re_tracking = cms.Sequence(process.bit40*
    ( process.siPixelRecHits * process.siStripMatchedRecHits ) *
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )


# re_reco
process.re_reco = cms.Sequence(process.bit40*
    ( process.siPixelRecHits * process.siStripMatchedRecHits ) *
    process.offlineBeamSpot *
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )

# re_fitting
process.refitting = cms.Sequence(process.bit40*
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )

### final path and endPath
process.p = cms.Path(process.SEQUENCE)
