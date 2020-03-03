# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --mc --eventcontent FEVT --runUnscheduled --customise SLHCUpgradeSimulations/Configuration/aging.customise_aging_1000 --datatier DIGI-RECO --conditions 106X_upgrade2023_realistic_v2 --step RAW2DIGI,RECO --nThreads 8 --geometry Extended2023D41 --era Phase2C8_timing_layer_bar --fileout file:step1.root --filein dbs:/DYToMuMuorEleEle_M-20_14TeV_pythia8/PhaseIITDRSpring19DR-NoPU_106X_upgrade2023_realistic_v3-v1/GEN-SIM-DIGI-RAW --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Phase2C8_timing_layer_bar_cff import Phase2C8_timing_layer_bar

process = cms.Process('RECO',Phase2C8_timing_layer_bar)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D41Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/E2C88213-1B8C-F740-BF2A-496508BF6F43.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/9202606D-2C69-2943-B10B-CE8BC4733799.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/759852D1-7D18-4847-9B6F-365410A52409.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/3F9C338D-0210-324A-8CE8-E91E8AC65910.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/1CCCCF3C-BCDC-9244-9ABE-8E9D18DF90EE.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/6743CFF1-81D2-DE4F-8D3B-2566DEDEEF97.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/F7CD660A-A280-854C-BA99-CFA574041CA1.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/E2512668-5B5F-D34B-BEAB-4996B8520A47.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/E34645D6-5A80-D94C-A195-B51FE23B941F.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/94DA07BC-B341-F145-8419-3672EB01AA65.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/079ECA04-2EE7-804F-8F50-5C0F8A9EA86D.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/1771D583-71A7-2A4B-BF27-F995642B804D.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/FF78D022-C799-8244-A249-B3D7251DE238.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/37C5A2A9-A78F-5942-A2F7-80C2E6FCB444.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/56484D0A-EF18-8246-ACC6-465BF3E746EF.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/76511DD3-F0E4-7D41-82D4-8A5BC563DDE7.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/F14BBDED-0352-4C43-96F3-A169428E93BB.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/6F5108FF-4945-324A-A67F-EE25D8B6B112.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/DE113561-61BA-8145-899F-3B4B9CADEC3F.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/BA496AE2-4F66-9A40-8494-D384F60BE15A.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/7B90AF52-A4FA-6B41-9517-C802E3C32F91.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/57D7FC14-E8F0-E640-9D96-EB8ECC1D2A95.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/CA9FCEE2-577D-F745-B9B5-7A9F8AE3FA4E.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/7BB7F8BF-4A12-B845-9834-41E6F3CF72FF.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/C091F8BC-4EDA-2E48-A21C-E48BB2CBC279.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/5E70527C-CD4B-9042-AF5F-3283DAE6F3B5.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/99DB2E54-1BDD-F943-96F3-3CF6CDFC7A44.root', 
        '/store/mc/PhaseIITDRSpring19DR/DYToMuMuorEleEle_M-20_14TeV_pythia8/GEN-SIM-DIGI-RAW/NoPU_106X_upgrade2023_realistic_v3-v1/40000/C4D37970-BF55-F64F-BAC2-684171E0EEAD.root'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DIGI-RECO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step1.root'),
    outputCommands = process.FEVTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2023_realistic_v2', '')

process.load('RecoLocalMuon.GEMCSCSegment.gemcscSegments_cfi')


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVToutput_step = cms.EndPath(process.FEVToutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.FEVToutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(8)
process.options.numberOfStreams=cms.untracked.uint32(0)
process.options.numberOfConcurrentLuminosityBlocks=cms.untracked.uint32(1)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.aging
from SLHCUpgradeSimulations.Configuration.aging import customise_aging_1000 

#call to customisation function customise_aging_1000 imported from SLHCUpgradeSimulations.Configuration.aging
process = customise_aging_1000(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
