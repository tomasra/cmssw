import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://xrootd-cms.infn.it//store/mc/RunIIISummer24PrePremix/Neutrino_E-10_gun/PREMIX/Premixlib2024_140X_mcRun3_2024_realistic_v26-v1/140007/6b22f430-90d3-4e66-99f1-7fccdae45317.root'
    )
)

process.simHitCSVWriter = cms.EDAnalyzer("SimHitCSVWriter",
    inputTag = cms.InputTag("mix", "g4SimHitsMuonCSCHits", "DIGI"),
    outputFile = cms.untracked.string("muon_csc_simhits.csv")
)

process.p = cms.Path(process.simHitCSVWriter)
process.schedule = cms.Schedule(process.p)