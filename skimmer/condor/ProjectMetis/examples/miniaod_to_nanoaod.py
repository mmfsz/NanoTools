from metis.CMSSWTask import CMSSWTask
from metis.Sample import DBSSample
from metis.StatsParser import StatsParser
import time

if __name__ == "__main__":

    total_summary = {}
    for _ in range(10000):

        nano = CMSSWTask(       
                sample = DBSSample(
                    dataset="/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM",
                    ),
                events_per_output = 150e3,
                output_name = "output.root",
                tag = "v1",
                pset = "../scratch/pset_nano.py",
                cmssw_version = "CMSSW_10_2_11",
                scram_arch = "slc6_amd64_gcc700",
                )

        nano.process()
        total_summary[nano.get_sample().get_datasetname()] = nano.get_task_summary()

        StatsParser(data=total_summary, webdir="~/public_html/dump/metis_nano/").do()
        time.sleep(30*60)
