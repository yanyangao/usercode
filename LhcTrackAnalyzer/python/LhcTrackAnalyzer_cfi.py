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
                                  secTrackCollectionTag = cms.InputTag("ctfTracksPixelLess")
                                  )

