## Installation and Setup
```bash
git clone https://github.com/aminnj/ProjectMetis/
cd ProjectMetis
source setup.sh
cd examples/nanoaod/
python submit.py
```

Relevant files
- pyROOT looper (`looper.py`) that makes a histogram based on one branch
- condor executable (`condor_nano_exe.sh`) which is run on the condor node
- python submission script (`submit.py`) which uses the ProjectMetis API to submit/resubmit/monitor jobs


This should submit a handful of jobs to run on one dataset. Check out the web dashboard to get more information
by clicking on dataset names.


