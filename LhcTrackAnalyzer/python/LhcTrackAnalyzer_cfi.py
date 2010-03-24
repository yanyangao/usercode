import FWCore.ParameterSet.Config as cms

LhcTrackAnalyzer = cms.EDAnalyzer("LhcTrackAnalyzer",
                                  ctfTrackCollectionTag = cms.InputTag("generalTracks"),
                                  vertexCollection = cms.untracked.InputTag('offlinePrimaryVertices'),
                                  pixelVertexCollectionTag =  cms.InputTag("pixelVertices"),
                                  beamSpot = cms.InputTag("offlineBeamSpot"), 
                                  OutputFileName = cms.string("LhcTrackAnalyzer.root"),
                                  Debug = cms.bool(False),
                                  afterRefitting  = cms.bool(False),
                                  runSecTrackColl = cms.bool(False),
                                  secTrackCollectionTag = cms.InputTag("ctfPixelLess")
                                  )

