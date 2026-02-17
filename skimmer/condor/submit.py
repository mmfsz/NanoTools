import os

from metis.Sample import DBSSample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser
import samples
import argparse

from time import sleep
import sys

condorpath = os.path.dirname(os.path.realpath(__file__))

# Avoid spamming too many short jobs to condor
# Less dileptn pairs = faster = more input files per job
def split_func(dsname):
    return 1

def njobs_to_process(dsname):
    return -1

if __name__ == "__main__":

    # Samples
    samples = samples.samples_to_submit

    # submission tag
    analysis_tags = [
#        "4Lep",
        "3Lep",
#        "2Lep2FJ",
#        "2Lep1FJ",
#        "1Lep1FJ",
#        "0Lep3FJ",
#        "0Lep2FJ",
#        "0Lep1FJ",
#        "0Lep0FJ"
    ]

    signal_flags = "" #"--dump_truth --is_signal" #leave "" for no flags

    # Task summary for printing out msummary
    task_summary = {}

    # To skip tail events
    skip_tail = False

    # Infinite loop until all tasks are complete
    # It will sleep every 10 minutes (600 seconds) and re-check automatically
    while True:

        # Boolean to aggregate whether all tasks are complete
        all_tasks_complete = True

        for analysis_tag in analysis_tags:
            tag = "nanoaodv9_bkg_" + analysis_tag + "_16Feb2026"
            # Loop over the dataset provided by the user few lines above, and do the Metis magic
            for ds in samples:
                task = CondorTask(
                        sample = ds,
                        files_per_output = split_func(ds.get_datasetname()),
                        output_name = "output.root",
                        tag = tag,
                        condor_submit_params = {
                            #"sites": "T2_US_UCSD", #UAF
                            "use_xrootd":True,
                            #"metis_retries": 3, does not work? 
                            "classads": [
                                ["metis_extraargs", signal_flags+" -d ./ -a "+analysis_tag+" -t Events -T Events"]
                                ]
                            },
                        max_jobs = njobs_to_process(ds.get_datasetname()), #FIXME 
                        cmssw_version = "CMSSW_10_2_13",
                        scram_arch = "slc7_amd64_gcc700",
                        input_executable = "{}/condor_executable_metis.sh".format(condorpath), # your condor executable here #FIXME 
                        tarfile = "{}/package.tar.xz".format(condorpath), # your tarfile with assorted goodies here
                        special_dir = "skim/{}".format(tag), # output files into /hadoop/cms/store/<user>/<special_dir>
                        min_completion_fraction = 0.50 if skip_tail else 1.0,
                )
                # Straightforward logic
                if not task.complete():
                    task.process()

                # Aggregate whether all tasks are complete
                all_tasks_complete = all_tasks_complete and task.complete()

                # Set task summary
                task_summary["%s_%s" % (task.get_sample().get_datasetname(), analysis_tag)] = task.get_task_summary()
                #task_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

        ##########
        # Parse the summary and make a summary.txt that will be used to pretty status of the jobs
        os.system("rm web_summary.json")
        webdir="~/public_html/skim_3Ltruth"
        StatsParser(data=task_summary, webdir=webdir).do()
        os.system("chmod -R 755 {}".format(webdir))
        os.system("msummary -r -i {}/web_summary.json".format(webdir))

        # If all done exit the loop
        if all_tasks_complete:
            print("")
            print("All job finished")
            print("")
            break

        # Neat trick to not exit the script for force updating
        print('Press Ctrl-C to force update, otherwise will sleep for 600 seconds')
        try:
            for i in reversed(range(0, 600)):
                sleep(1) # could use a backward counter to be preeety :)
                sys.stdout.write("\r{} mins {} seconds till updating ...".format(i/60, i%60))
                sys.stdout.flush()
        except KeyboardInterrupt:
            raw_input("Press Enter to force update, or Ctrl-C to quit.")
            print("Force updating...")

