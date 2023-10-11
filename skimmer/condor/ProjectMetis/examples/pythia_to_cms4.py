from metis.CMSSWTask import CMSSWTask
from metis.Sample import DirectorySample, DummySample
from metis.StatsParser import StatsParser
import time

import numpy as np

tag = "v1"
total_summary = {}
for _ in range(10000):

    # first tast has no input files, just make GENSIM from a fragment with pythia commands
    gen = CMSSWTask(       
            sample = DummySample(N=1, dataset="/WH_HtoRhoGammaPhiGamma/privateMC_102x/GENSIM"),
            events_per_output = 1000,
            total_nevents = 1000000,
            pset = "gensim_cfg.py",
            cmssw_version = "CMSSW_10_2_5",
            scram_arch = "slc6_amd64_gcc700",
            tag = tag,
            split_within_files = True,
            )

    raw = CMSSWTask(
            sample = DirectorySample(
                location = gen.get_outputdir(),
                dataset = gen.get_sample().get_datasetname().replace("GENSIM","RAWSIM"),
                ),
            open_dataset = True,
            files_per_output = 1,
            pset = "rawsim_cfg.py",
            cmssw_version = "CMSSW_10_2_5",
            scram_arch = "slc6_amd64_gcc700",
            tag = tag,
            )

    aod = CMSSWTask(
            sample = DirectorySample(
                location = raw.get_outputdir(),
                dataset = raw.get_sample().get_datasetname().replace("RAWSIM","AODSIM"),
                ),
            open_dataset = True,
            files_per_output = 5,
            pset = "aodsim_cfg.py",
            cmssw_version = "CMSSW_10_2_5",
            scram_arch = "slc6_amd64_gcc700",
            tag = tag,
            )

    miniaod = CMSSWTask(
            sample = DirectorySample(
                location = aod.get_outputdir(),
                dataset = aod.get_sample().get_datasetname().replace("AODSIM","MINIAODSIM"),
                ),
            open_dataset = True,
            flush = True,
            files_per_output = 5,
            pset = "miniaodsim_cfg.py",
            cmssw_version = "CMSSW_10_2_5",
            scram_arch = "slc6_amd64_gcc700",
            tag = tag,
            )

    cms4 = CMSSWTask(
            sample = DirectorySample(
                location = miniaod.get_outputdir(),
                dataset = miniaod.get_sample().get_datasetname().replace("MINIAODSIM","CMS4"),
                ),
            open_dataset = True,
            flush = True,
            files_per_output = 1,
            output_name = "merged_ntuple.root",
            pset = "psets_cms4/main_pset_V10-02-04.py",
            pset_args = "data=False year=2018",
            global_tag = "102X_upgrade2018_realistic_v12",
            cmssw_version = "CMSSW_10_2_5",
            scram_arch = "slc6_amd64_gcc700",
            tag = tag,
            tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V10-02-04_1025.tar.xz",
            )

    tasks = [gen,raw,aod,miniaod,cms4]

    for task in tasks:
        task.process()
        summary = task.get_task_summary()
        total_summary[task.get_sample().get_datasetname()] = summary

    StatsParser(data=total_summary, webdir="~/public_html/dump/metis/").do()
    time.sleep(30*60)
