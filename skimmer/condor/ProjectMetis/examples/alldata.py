from __future__ import print_function

import time
import itertools
import json
import traceback

from metis.Sample import DBSSample
from metis.CMSSWTask import CMSSWTask
from metis.StatsParser import StatsParser
from metis.Utils import send_email

if __name__ == "__main__":


    pds = ["MuonEG","SingleElectron","MET","SinglePhoton","SingleMuon","DoubleMuon","JetHT","DoubleEG","HTMHT"]
    proc_vers = [
            # ("Run2017B","v1"),
            # ("Run2017B","v2"),
            # ("Run2017C","v1"),
            # ("Run2017C","v2"),
            # ("Run2017C","v3"),
            # ("Run2017D","v1"),
            # ("Run2017E","v1"),
            ("Run2017F","v1"),
            ]
    dataset_names =  ["/{0}/{1}-PromptReco-{2}/MINIAOD".format(x[0],x[1][0],x[1][1]) for x in itertools.product(pds,proc_vers)]


    for i in range(10000):

        total_summary = {}
        total_counts = {}
        for dsname in dataset_names:


            open_dataset = False

            cmsswver = "CMSSW_9_2_7_patch1"
            tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-06.tar.gz"

            if "2017C-PromptReco-v2" in dsname: 
                open_dataset = False

            if "2017C-PromptReco-v3" in dsname: 
                cmsswver = "CMSSW_9_2_8"
                tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-06_928.tar.gz"
                open_dataset = False

            if "2017D-PromptReco-v1" in dsname: 
                cmsswver = "CMSSW_9_2_10"
                tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-06_9210.tar.gz"
                open_dataset = False

            if "2017E-PromptReco-v1" in dsname: 
                cmsswver = "CMSSW_9_2_12"
                tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-06_9212.tar.gz"
                open_dataset = False

            if "2017F-PromptReco-v1" in dsname: 
                cmsswver = "CMSSW_9_2_13"
                tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-06_9213.tar.gz"
                open_dataset = False

            try:

                task = CMSSWTask(
                        sample = DBSSample(dataset=dsname),
                        open_dataset = open_dataset,
                        flush = ((i+1)%48==0), 
                        # flush = ((i)%48==0), 
                        events_per_output = 450e3,
                        output_name = "merged_ntuple.root",
                        tag = "CMS4_V00-00-06",
                        global_tag = "", # if global tag blank, one from DBS is used
                        pset = "main_pset.py",
                        pset_args = "data=True prompt=True",
                        cmssw_version = cmsswver,
                        condor_submit_params = {"use_xrootd":True},
                        tarfile = tarfile,
                        is_data = True,
                        publish_to_dis = True,
                )
            
                task.process()
            except:
                traceback_string = traceback.format_exc()
                print("Runtime error:\n{0}".format(traceback_string))
                send_email(subject="metis error", body=traceback_string)


            total_summary[dsname] = task.get_task_summary()

        StatsParser(data=total_summary, webdir="~/public_html/dump/metis/", make_plots=False).do()

        # time.sleep(1.*3600)
        time.sleep(60.*60)

