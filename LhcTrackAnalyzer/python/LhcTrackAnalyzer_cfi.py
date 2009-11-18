import FWCore.ParameterSet.Config as cms

LhcTrackAnalyzer = cms.EDAnalyzer("LhcTrackAnalyzer",
                                  TrackCollectionTag = cms.InputTag("generalTracks"),
                                  vertexCollection = cms.untracked.InputTag('offlinePrimaryVertices'),
                                  beamSpot = cms.InputTag("offlineBeamSpot"), 
                                  OutputFileName = cms.string("LhcTrackAnalyzer.root"),
                                  Debug = cms.bool(False)
                                  )

