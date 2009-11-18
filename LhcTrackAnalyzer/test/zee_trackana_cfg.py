import FWCore.ParameterSet.Config as cms

process = cms.Process("pvtxana")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.Geometry_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#======================================
# Global Tag
#======================================
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag ='MC_31X_V3::All'
process.prefer("GlobalTag")

#======================================
# Input
#======================================
process.load("UserCode.LhcTrackAnalyzer.Zee_Summer09_MC_31X_V3_cff")
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(10000) )

#======================================
# TrackTupleMaker
#======================================
process.load("UserCode.LhcTrackAnalyzer.LhcTrackAnalyzer_cff")
process.trackana = process.LhcTrackAnalyzer.clone()
process.trackana.Debug = False
process.trackana.OutputFileName = cms.string("work/LhcTrackAnalyzer.root")

#======================================
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) ## default is false
process.allPath=cms.Path(process.trackana)
