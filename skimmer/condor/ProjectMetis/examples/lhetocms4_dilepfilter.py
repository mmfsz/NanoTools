from metis.CMSSWTask import CMSSWTask
from metis.Sample import DirectorySample
from metis.StatsParser import StatsParser
import time

import numpy as np

ktstrs = [str(round(kt,1)).replace(".","p") for kt in np.arange(0.4,2.2,0.1)]
# ktstrs = [str(round(kt,1)).replace(".","p") for kt in np.arange(0.4,0.5,0.1)]
runstrs = ["{:02d}".format(i) for i in range(1,len(ktstrs)+1)]

total_summary = {}
for _ in range(25):
    for runstr,ktstr in zip(runstrs,ktstrs):
        lhe = CMSSWTask(
                sample = DirectorySample(
                    location="/hadoop/cms/store/user/namin/yukawa_lhe/Events/run_{}/".format(runstr),
                    globber="*.lhe",
                    dataset="/tttt-LOytscan/kt-{}_v2/LHE".format(ktstr),
                    ),
                events_per_output = 100,
                total_nevents = 10000,
                pset = "pset_gensim.py",
                cmssw_version = "CMSSW_7_1_25_patch2",
                split_within_files = True,
                )

        raw = CMSSWTask(
                sample = DirectorySample(
                    location = lhe.get_outputdir(),
                    dataset = lhe.get_sample().get_datasetname().replace("LHE","RAW"),
                    ),
                open_dataset = True,
                files_per_output = 1,
                pset = "pset_raw.py",
                cmssw_version = "CMSSW_8_0_21",
                )

        aod = CMSSWTask(
                sample = DirectorySample(
                    location = raw.get_outputdir(),
                    dataset = raw.get_sample().get_datasetname().replace("RAW","AOD"),
                    ),
                open_dataset = True,
                files_per_output = 5,
                pset = "pset_aod.py",
                cmssw_version = "CMSSW_8_0_21",
                )

        miniaod = CMSSWTask(
                sample = DirectorySample(
                    location = aod.get_outputdir(),
                    dataset = aod.get_sample().get_datasetname().replace("AOD","MINIAOD"),
                    ),
                open_dataset = True,
                flush = True,
                files_per_output = 10,
                pset = "pset_miniaod.py",
                cmssw_version = "CMSSW_8_0_21",
                )

        cms4 = CMSSWTask(
                sample = DirectorySample(
                    location = miniaod.get_outputdir(),
                    dataset = miniaod.get_sample().get_datasetname().replace("MINIAOD","CMS4"),
                    ),
                open_dataset = True,
                flush = True,
                files_per_output = 20,
                output_name = "merged_ntuple.root",
                tag = "CMS4_V00-00-02_2017Sep27",
                pset = "pset_moriondremc.py",
                pset_args = "data=False",
                global_tag = "80X_mcRun2_asymptotic_2016_TrancheIV_v6",
                condor_submit_params = {"use_xrootd":True},
                # condor_submit_params = {"sites":"T2_US_UCSD"},
                cmssw_version = "CMSSW_8_0_26_patch1",
                tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-02_2017Sep27.tar.gz",
                special_dir = "run2_moriond17_cms4/ProjectMetis",
                )

        tasks = [lhe,raw,aod,miniaod,cms4]

        for task in tasks:
            task.process()
            summary = task.get_task_summary()
            total_summary[task.get_sample().get_datasetname()] = summary

    StatsParser(data=total_summary, webdir="~/public_html/dump/metis_ytscan/").do()
    time.sleep(2.0*3600)
