import FWCore.ParameterSet.Config as cms
process = cms.Process("DUMMY")
process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.GlobalTag.globaltag = "94X_mc2017_realistic_v14"
process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('ntuple.root'),
        dropMetaData = cms.untracked.string("ALL"),
        )
process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(
            '/store/user/namin/test/68753E9C-6D5E-E811-BC40-24BE05C4D821.root',
            )
        )
process.Timing = cms.Service("Timing",
        summaryOnly = cms.untracked.bool(True)
        )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.out.outputCommands = cms.untracked.vstring( 'drop *' )
process.outpath = cms.EndPath(process.out)
