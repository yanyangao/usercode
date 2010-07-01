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

# suppress report message every 1000 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.options = cms.untracked.PSet(
        Rethrow = cms.untracked.vstring('ProductNotFound'),
            wantSummary = cms.untracked.bool(False)
        )

# Input source
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
''
                     ]);
secFiles.extend( [ ] )
                             
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(50000))

# Other statements
process.GlobalTag.globaltag = "GR10_P_V6::All"

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
process.trackana.OutputFileName = cms.string("ntuple.root")
process.trackana.vertexCollection = "offlinePrimaryVertices"
process.trackana.pixelVertexCollectionTag = "pixelVertices"


#======================================
# Trigger Filter
#======================================
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
process.bit40 = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39) AND NOT ((42 AND NOT 43) OR (43 AND NOT 42))')
    )

# require scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
                                    applyfilter = cms.untracked.bool(True),
                                    debugOn = cms.untracked.bool(False),
                                    numtrack = cms.untracked.uint32(10),
                                    thresh = cms.untracked.double(0.25)
                                    )

process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                           src = cms.InputTag("offlinePrimaryVertices"),
                                           cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
                                           filter = cms.bool(True),
                                           # otherwise it won't filter the events, just produce an empty vertex collection.
                                           )

process.GOODCOLL = cms.Sequence(process.bit40*(process.primaryVertexFilter+process.scrapingVeto))



# only_analyze
process.only_analyze = cms.Sequence(process.trackana)   

# re_tracking
process.re_tracking = cms.Sequence(
    ( process.siPixelRecHits * process.siStripMatchedRecHits ) *
    process.ckftracks *
    process.ctfTracksPixelLess
    * process.trackana
    )


# re_reco
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
process.p = cms.Path(process.GOODCOLL*process.only_analyze)

