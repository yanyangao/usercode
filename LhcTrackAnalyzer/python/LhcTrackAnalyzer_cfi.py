import FWCore.ParameterSet.Config as cms

LhcTrackAnalyzer = cms.EDAnalyzer("LhcTrackAnalyzer",
                                  ctfTrackCollectionTag = cms.InputTag("generalTracks"),
                                  vertexCollection = cms.untracked.InputTag('offlinePrimaryVertices'),
                                  beamSpot = cms.InputTag("offlineBeamSpot"), 
                                  OutputFileName = cms.string("LhcTrackAnalyzer.root"),
                                  Debug = cms.bool(False),
                                  afterRefitting  = cms.bool(False),
                                  saveAllClusters = cms.bool(False),
                                  runSecTrackColl = cms.bool(False),
                                  secTrackCollectionTag = cms.InputTag("ctfPixelLess"),
                                  selTechBit = cms.bool(False), # if select event based on TechBit
                                  techBitToSelect = cms.int32(0), # select a certain techBit
                                  selNonFakePvtx = cms.bool(False) # if select event based if there is non-fake pvtx
                                  )

