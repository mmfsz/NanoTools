## Running the SNT NtupleMaker with Metis

### First things first...
Visit the [NtupleMaker repository](https://github.com/cmstas/NtupleMaker) and check out the README there to set up a local version. Make sure that it works for a given file, and take note of the cmsRun flags needed, as well as the pset configuration file location.

### Now, some submission fun with Metis
Check out this repository, `source setup.sh`. Now go back into your local copy of NtupleMaker, run `cmsenv` for good measure, and now run the command
`mtarfile tarfile.tar.gz` (replacing the name of the tarfile with whatever you want). This packages the CMSSW code (including NtupleMaker of course)
into a tarfile to be shipped off to worker nodes. Take note of the tarfile location.

Note: temporarily, you may have to use 
```
mtarfile tarfile.tar.gz -e $CMSSW_BASE/external/slc6_amd64_gcc630/lib/libmxnet.so \
  $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/{mxnet,openblas}.xml \
  $CMSSW_BASE/src/NNKit/data/{preprocessing.json,resnet-symbol.json,resnet.params}
```
to include things for DeepAK8 until it gets integrated into CMSSW.

Now make a python script to serve as a job manager/submitter and put in the below content.
Feel free to read the comments.

Ignoring the excessive comments and unnecessary stuff, it is <20 lines! And since this is Python code,
you can of course script this to your heart's content (e.g., loop over several datasets to submit them).
<details>
<summary>Click to show script</summary>

```python
from metis.Sample import DBSSample
from metis.CMSSWTask import CMSSWTask
from metis.StatsParser import StatsParser

def main():

    # Metis has the concept of a Task object and a Sample object
    # A Task object operates on a single Sample object in the way you'd expect
    task = CMSSWTask(
            # A DBSSample object takes a dataset name and queries DBS to get file names and event counts
            sample = DBSSample(dataset="/ZeroBias6/Run2017A-PromptReco-v2/MINIAOD"),
            # Is the dataset "open"? For datasets that are still accumulating files,
            # set this to True to requery the filelist every time and pick up new ones.
            # When the dataset is complete, set it back to False (the default) to flush
            # remaining files.
            open_dataset = False,
            # What should the output file name be?
            output_name = "merged_ntuple.root",
            # You can specify the number of events per output (or the number of 
            # input files per output file: `files_per_output`)
            events_per_output = 450e3,
            # Specify a unique submission tag for bookkeeping
            tag = "testv1",
            # Put in the pset I told you to note above
            pset = "pset_test.py",
            # Put in the *full path* to the tarfile made previously
            tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-03_workaround.tar.gz",
            # Not really needed - just used for optimizations for central SNT ntuple production
            # E.g., if an ntuple is data, don't compute the sum of weights (genweights) when a job finishes
            is_data = True,
            # Put in the cmsRun arguments I told you to note above (or empty if none)
            pset_args = "data=True prompt=True",
            # Next two are self-explanatory (this scram_arch value is the default)
            cmssw_version = "CMSSW_9_2_1",
            scram_arch = "slc6_amd64_gcc530",

    )
    
    # Do pretty much everything
    #  - get list of files (or new files that have appeared)
    #  - chunk inputs to construct outputs
    #  - submit jobs to condor
    #  - resubmit jobs that fail
    task.process()

    # Get a nice json summary of files, event counts, 
    # condor job resubmissions, log file locations, etc.
    # and push it to a web area (with dashboard goodies)
    StatsParser(data=total_summary, webdir="~/public_html/dump/metis_test/").do()

if __name__ == "__main__":

    # Do stuff, sleep, do stuff, sleep, etc.
    for i in range(100):
        main()

        # 1 hr power nap so we wake up refreshed
        # and ready to process some more data
        time.sleep(1.*3600)

        # Since everything is backed up, totally OK to Ctrl+C and pick up later
```

</details>



