# ProjectMetis

[![Build Status](https://travis-ci.org/aminnj/ProjectMetis.png)](https://travis-ci.org/aminnj/ProjectMetis)
[![Coverage Status](https://coveralls.io/repos/github/aminnj/ProjectMetis/badge.png)](https://coveralls.io/github/aminnj/ProjectMetis)

ProjectMetis is a workflow manager that can submit somewhat arbitrary batch jobs to HTCondor.
An input can be a list of files or a DBS-published dataset. Tasks can be chained and failures
are resubmitted automatically. There's also a pretty monitoring dashboard.

<img src="dashboard/images/dashboard.png">

## Installation and Setup
Clone this repository, and set up the environment via `source setup.sh`. Note that this doesn't overwrite an existing CMSSW environment if you already have one.

To check that things are working (including condor submission, job stageout, proxy permissions), run the unit tests with `mtest`.

A minimal setup to submit jobs to run on CMS NANOAOD is in [`examples/nanoaod/`](examples/nanoaod/).

## Example
CRAB-like operation (`CMSSWTask`) requires a dataset name, a CMSSW pset, and a tarball of the environment (if necessary).
Here's a quick preview, but there are examples with different use cases examples in the [examples directory](examples/).

```python
from metis.CMSSWTask import CMSSWTask
from metis.Sample import DBSSample
from metis.StatsParser import StatsParser
import time

def run():
    total_summary = {}
    for dsname in [
                "/SingleMuon/Run2017H-17Nov2017-v2/MINIAOD",
                "/DoubleMuon/Run2017H-17Nov2017-v1/MINIAOD",
            ]:
        task = CMSSWTask(
                sample = DBSSample(dataset=dsname),
                events_per_output = 700e3,
                output_name = "output.root",
                tag = "v1",
                pset = "pset_NANO_from_MINIAOD.py",
                cmssw_version = "CMSSW_10_2_5",
                scram_arch = "slc6_amd64_gcc700",
                # Optionally specify a tarball of the CMSSW environment made with `mtarfile`
                # tarfile = "/nfs-7/userdata/libCMS3/lib_CMS4_V00-00-03_workaround.tar.gz",
                )

        # Chunk inputs, submit to condor, resubmit failures, etc
        task.process()

        total_summary[task.get_sample().get_datasetname()] = task.get_task_summary()

    # Web dashboard
    StatsParser(data=total_summary, webdir="~/public_html/dump/metis_nano/").do()

if __name__ == "__main__":
    for i in range(100):
        run()
        time.sleep(30*60)
```



## Unit tests
The full unit test suite is run using the executable `mtest` in `scripts/` (if Metis is set up properly, you need only execute the command `mtest`).

## Development
General workflow is 
* Make changes
* Test with `mtest` (or if it's a minor change, ignore this and let the continuous integration take care of testing)
* Submit a PR

