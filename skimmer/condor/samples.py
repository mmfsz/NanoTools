from metis.Sample import DirectorySample, DBSSample

# Master list of all samples
# Specify a dataset name and a short name for the output root file on nfs

testsamples = [

    # Data
    DBSSample(dataset="/Muon/Run2022G-PromptNanoAODv11_v1-v2/NANOAOD"),
    DBSSample(dataset="/EGamma/Run2022G-PromptNanoAODv11_v1-v2/NANOAOD"),
    DBSSample(dataset="/MuonEG/Run2022G-PromptNanoAODv11_v1-v2/NANOAOD"),

    # MC
    DBSSample(dataset="/DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22EENanoAODv11-126X_mcRun3_2022_realistic_postEE_v1-v1/NANOAODSIM"),
    DBSSample(dataset="/WW_TuneCP5_13p6TeV_pythia8/Run3Summer22EENanoAODv11-126X_mcRun3_2022_realistic_postEE_v1-v1/NANOAODSIM"),
    DBSSample(dataset="/WtoLNu-2Jets_TuneCP5_13p6TeV_amcatnloFXFX-pythia8/Run3Summer22EENanoAODv11-126X_mcRun3_2022_realistic_postEE_v1-v1/NANOAODSIM"),
    DBSSample(dataset="/TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8/Run3Summer22EENanoAODv11-126X_mcRun3_2022_realistic_postEE_v1-v1/NANOAODSIM"),

]
