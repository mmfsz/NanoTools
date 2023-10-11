import os
import time

from metis.CondorTask import CondorTask
from metis.Sample import DBSSample
from metis.StatsParser import StatsParser

if not os.path.exists("inputs.tar.gz"):
    os.system("tar cvzf inputs.tar.gz looper.py")

for i in range(100):
    total_summary = {}
    for dataset in [
            "/TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM",
            "/DoubleMuon/Run2018D-UL2018_MiniAODv1_NanoAODv2-v1/NANOAOD",
            ]:
        task = CondorTask(
                sample = DBSSample(dataset=dataset),
                events_per_output = 3e6,
                output_name = "output.root",
                tag = "nanotestv4",
                cmssw_version = "CMSSW_10_6_19_patch3",
                scram_arch = "slc7_amd64_gcc700",
                tarfile = "inputs.tar.gz",
                executable = "condor_nano_exe.sh",
                )
        task.process()
        total_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

    StatsParser(data=total_summary, webdir="~/public_html/dump/metis_nanotest/").do()
    time.sleep(30*60)
