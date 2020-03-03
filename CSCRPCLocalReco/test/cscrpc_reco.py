## config to run CSCDumpLocaReco - Tim Cox - 10.07.2019

import FWCore.ParameterSet.Config as cms
process = cms.Process("DUMPCSC")

## 2017 geometry 
#from Configuration.Eras.Era_Run2_2017_cff import Run2_2017
## Phase 2
from Configuration.Eras.Era_Phase2C8_timing_layer_bar_cff import Phase2C8_timing_layer_bar
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')

## Global tag for 10_6 phase2 mc
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2023_realistic_v2', '')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring(
     'file:step1.root' )
)

process.TFileService = cms.Service("TFileService",
	fileName = cms.string("cscrpc.root"),
	closeFileFast = cms.untracked.bool(True))



process.dumpcsc = cms.EDAnalyzer('CSCRPCLocalReco',

   dumpDigis = cms.untracked.bool(False),
   dumpRechits = cms.untracked.bool(True),
   dumpSegments = cms.untracked.bool(True),
   isSegmentMatchFilter = cms.untracked.bool(True),
   CheckType = cms.untracked.uint32(1),
   SampleType = cms.untracked.uint32(1),
   TrackType = cms.untracked.int32(13),
   isCSCSegmentFilter = cms.untracked.bool(True),
   deltaRTH = cms.untracked.double(0.2),
   ConeAngleX = cms.untracked.double(0.1),
   ConeAngleY = cms.untracked.double(-0.1),
   isEtaFilter = cms.untracked.bool(True),
   MinSegmentEta = cms.untracked.double(-2.4),
   MaxSegmentEta = cms.untracked.double(2.4),
   MaxD = cms.untracked.double(80.0),
   DuplicationCorrection = cms.untracked.int32(1),
   RangeStrips = cms.untracked.double(1.),
   ResidualDistanceTH = cms.untracked.double(5.0),
   RPCDigiTag = cms.InputTag("muonRPCDigis"),
   RPCrecHitTag = cms.InputTag("rpcRecHits"),
   cscRecHitTag = cms.InputTag("csc2DRecHits"),
   cscSegTag = cms.InputTag("cscSegments"),
   rpcRecHitTag = cms.InputTag("rpcRecHits")
)




process.p = cms.Path(process.dumpcsc)
