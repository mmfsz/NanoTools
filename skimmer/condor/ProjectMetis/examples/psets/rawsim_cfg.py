# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --filein gensim.root --fileout file:rawsim.root --pileup_input dbs:/Neutrino_E-10_gun/RunIISummer17PrePremix-PUAutumn18_102X_upgrade2018_realistic_v15-v1/GEN-SIM-DIGI-RAW --mc --eventcontent PREMIXRAW --datatier GEN-SIM-RAW --conditions 102X_upgrade2018_realistic_v15 --step DIGI,DATAMIX,L1,DIGI2RAW,HLT:@relval2018 --procModifiers premix_stage2 --nThreads 8 --geometry DB:Extended --datamix PreMix --era Run2_2018 --python_filename rawsim_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2

process = cms.Process('HLT',eras.Run2_2018,premix_stage2)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.DigiDM_cff')
process.load('Configuration.StandardSequences.DataMixerPreMix_cff')
process.load('Configuration.StandardSequences.SimL1EmulatorDM_cff')
process.load('Configuration.StandardSequences.DigiToRawDM_cff')
process.load('HLTrigger.Configuration.HLT_2018v32_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:gensim.root'),
    inputCommands = cms.untracked.vstring(
        'keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.PREMIXRAWoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:rawsim.root'),
    outputCommands = process.PREMIXRAWEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mixData.input.fileNames = cms.untracked.vstring(['/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/1C56C5CD-24BC-A841-A5D5-FD70C468F890.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/ED3110CF-AF42-A94C-B382-E7620DF2EAF1.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/A57200A7-1FC6-C847-99CF-989399C2A0E3.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/C7D9B09B-C68E-3F4F-88A4-2777E2817A8F.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/0874D661-2F53-7848-B31B-D3E6BB10A9EE.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/00DC4CA2-CF53-C643-9312-4BF27CCACBB0.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/AB8A03C4-90FA-4A4C-BADE-6E37A6A2B340.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/71E226BA-0545-BC41-828F-A1DA727A2EA3.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/9DE67611-8483-A245-9F2A-845ADF0E6A77.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/1BFC37B8-6104-FC40-9A40-EDE989A310EE.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/9259D484-D3B3-6145-9B0F-569C8F35AC9B.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/400E4CF0-72BF-A140-A0EE-4DE15C5FCB80.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/0F50F83F-CE0D-004E-911C-6D58B6A96775.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/4DBD34EB-0865-2D45-8437-25933F4F751D.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/BADA12EB-164D-6344-9023-ACDF08771800.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/AEB0369B-0049-8347-894A-6A33CAD674B4.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/B8BFEC5A-B7C8-5B45-8554-518250BE7407.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/489A92AE-F975-654D-A9E6-D67FDCFD507C.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/B8401647-A264-3A41-81FD-EE860BB32D46.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/9ECE4946-3BE3-2343-BE50-A4A351BA6CC0.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/1B520F65-6F89-9843-A9DC-DB8ED0F88B5B.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/80A31710-E854-9342-8449-7430981E5F1A.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/CFCB586F-33CD-CF47-9C1B-A67D0D573EB6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/AF446931-1576-9E4D-BF88-215992A41E9F.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/AF467A5F-2403-C64F-A0A9-22A7D0467CB5.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/82DFF3B1-5C0C-1C49-A9B0-9CAAFAF91B9A.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/536E2BB4-9C56-C54D-B480-E7169B4006AF.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/C540A8E5-5ED4-2F40-95C9-340CC32AFAC3.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/5A249C76-8C1D-0D47-A7FA-AEAC15D29784.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/C3D97F28-55D2-C84B-AA02-E1DA10B2D378.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/5C537A2C-5C8D-1843-B775-4BE2249ECA33.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/A0FF73E8-CD67-3A45-A124-FB52537D0B98.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/953485B6-0820-814C-82BD-8A72EF4EC790.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/DDFC594C-ED59-6248-A233-42666CD1A347.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/4B092FF2-DEAA-7E4E-9FCC-A083BD78D1A6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/A1133035-15B7-9948-BC7E-0A14F913FD50.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/2B4ECEFC-FF36-1A4B-B692-E1FC6D1E6781.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/6723AABA-CB2B-494B-A026-D40F1A64C378.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/26AB807A-21AB-1542-8545-DCB8E93C5AFD.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/1F4F63C1-F8F2-124B-A826-D3F7979E9B35.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/BC095F08-714E-BA4B-8B99-605983C76468.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/698E4C47-526A-E240-A437-D43B6272925C.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/7607EE0B-70E5-F34A-B814-B2F854DAD724.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/B500A1A4-1DF1-1146-914F-A66F59472BBE.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/B9BEF5A6-8329-AD43-ACFE-78743BE3701D.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/9D515840-4AFC-2D4B-AB02-EF45FD365088.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/33B8439A-BB81-6047-BFB4-B5F059B02C6E.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/9AB0AE16-8840-B040-B48E-250D9898AC25.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/AADDC714-F7D5-4E44-BC95-BF8ADD2A8309.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/4695B95D-3F8C-C346-B2F6-1808D196C607.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/3B7EC6F6-3746-4949-80D3-EE625F6F51EA.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00001/576C57E9-913E-CC44-95AB-CE0B117F6EE6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/7C5950D5-3142-C544-A583-BC4F32787F54.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/B8BD7C42-D04D-364E-8C81-0D712D51010A.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/EFDA2F2A-3FC0-5A43-AEEF-C04AC68F3D96.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/17E181DD-9E61-A24C-BF96-C2C5CB1E12C4.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/B2801E4E-A4FE-B44F-9AA2-C30DFD765D99.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/B4C82B40-303C-1445-9910-53501EC00374.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/25969B0D-EA5B-ED4B-9AF6-892135B1B9FB.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/98E29912-30E3-DB43-8299-D3224ED3B158.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/ACBCF669-02C2-5D49-A424-C98A4D321BF6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/6C533986-D140-6149-A9E5-B1633E174D2E.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/D5421E84-B9D3-3543-923A-C102FCB8F704.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/39CED810-3B07-FD49-B1E2-2640B1FAFDE3.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/15032D50-ECA6-2F4D-8873-E1995AE0890F.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/35F40417-18ED-4945-B8F3-B7FB01263835.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/12DB3F9E-FB70-3341-9584-52DA1FC79958.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/4C76D603-B1E4-1449-80AF-3DD5DC031042.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/8A4553B4-9A2A-834A-B964-C0CE9B1E5CD6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/55317857-7A75-A14E-89E7-694153395B8B.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/E9AB18BA-F6DD-504C-8EF3-7E16385CF4C5.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/9E1A75AB-5DC6-0545-B0C9-BA18A7A5FDE3.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/1B57C37C-AE4D-A648-8AD7-1E0AE08F748E.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/0AD48BC6-BFB9-A748-B27C-5505DA8D9BE6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/C3A14823-6D5E-0F45-803A-F55A15ED15EE.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/57BCA30C-EEFB-DD4D-A420-A92856E99FA6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/42C25447-3482-094A-ACC5-0336860FDF4C.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/C51E2049-BF83-5C4E-8FF2-1EB5D5750ED3.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/6AEB3581-52A4-3648-9DA8-CF1EF594AF4A.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/585E8232-64DC-1843-B71A-644681C1DD4D.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/B52753BD-CA6E-7148-B7BE-B79AA17A5239.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/CBFD9DE7-6F70-8B47-9AC4-25289B22C5F5.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/4AB179E0-D4F3-8A43-B4B7-54E60C255BF6.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/867B09E1-016A-A343-A03E-46FD7D5E7B33.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/A3825020-2532-734B-9F55-EA8F657D4F65.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/201ED1CB-2594-804C-9BB0-CF991FBE881B.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/398058B4-F6F0-1A41-9E73-50611B63F612.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/F2BAD88C-2B58-6D4E-A464-C813C6A52254.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/5FC23FAF-7A1B-D147-8643-444B971B4ADB.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/39684963-EB26-C34C-8586-C27AFA1FFB76.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/C904C362-642D-614E-A2BA-631E20480801.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00004/CEA6F33E-05F4-AA4D-B3F2-D4199E85D699.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/DA55E82A-2752-BB41-9FEA-D8F154EDE206.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00002/01905FF2-A64E-4549-B0E6-A4DA68C8CB57.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00000/A999CD86-D76A-AD4D-8274-2C634BA85982.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/3B8FB39F-D571-9344-81D6-7B5EAA2D9EC2.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/44BF01E8-4928-AB45-9058-C38A16748251.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/3FC6358F-282C-9E49-A467-B68C3251B548.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/3B5750DA-9F5A-2A4C-A7CD-967C550B75D2.root', '/store/mc/RunIISummer17PrePremix/Neutrino_E-10_gun/GEN-SIM-DIGI-RAW/PUAutumn18_102X_upgrade2018_realistic_v15-v1/00003/4BABCDF0-D6ED-CC4B-B387-F15445B53BDE.root'])
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '102X_upgrade2018_realistic_v15', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.datamixing_step = cms.Path(process.pdatamix)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.PREMIXRAWoutput_step = cms.EndPath(process.PREMIXRAWoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.datamixing_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.PREMIXRAWoutput_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(1)
process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
