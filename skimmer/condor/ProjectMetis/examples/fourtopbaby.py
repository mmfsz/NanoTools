import os

from metis.Sample import SNTSample
from metis.LocalMergeTask import LocalMergeTask
from metis.CondorTask import CondorTask

# Avoid spamming too many short jobs to condor
# Less dileptn pairs = faster = more input files per job
def split_func(dsname):
    if any(x in dsname for x in [
        "/W","/Z","/TTJets","/DY","/ST",
        ]):
        return 5
    elif "Run201" in dsname:
        return 7
    else:
        return 2

if __name__ == "__main__":

    # Specify a dataset name and a short name for the output root file on nfs
    sample_map = {
            "/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"    : "TTBAR_PH",
            "/TTTT_TuneCP5_PSweights_13TeV-amcatnlo-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM": "TTTTnew",
            "/TTTW_TuneCP5_13TeV-madgraph-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM"          : "TTTW",
            }

    # submission tag
    tag = "v1_PMtest"

    merged_dir = "/nfs-7/userdata/{}/tupler_babies/merged/FT/{}/output/".format(os.getenv("USER"),tag)
    for dsname,shortname in sample_map.items():
        task = CondorTask(
                sample = SNTSample(
                    dataset=dsname,
                    # tag="CMS4_V09-04-13", # if not specified, get latest CMS4 tag
                    ),
                files_per_output = split_func(dsname),
                output_name = "output.root",
                tag = tag,
                condor_submit_params = {"use_xrootd":True},
                cmssw_version = "CMSSW_9_2_8",
                input_executable = "inputs/condor_executable_metis.sh", # your condor executable here
                tarfile = "inputs/package.tar.xz", # your tarfile with assorted goodies here
                special_dir = "FTbabies/", # output files into /hadoop/cms/store/<user>/<special_dir>
        )
        # When babymaking task finishes, fire off a task that takes outputs and merges them locally (hadd)
        # into a file that ends up on nfs (specified by `merged_dir` above)
        merge_task = LocalMergeTask(
                input_filenames=task.get_outputs(),
                output_filename="{}/{}.root".format(merged_dir,shortname)
                )
        # Straightforward logic
        if not task.complete():
            task.process()
        else:
            if not merge_task.complete():
                merge_task.process()

