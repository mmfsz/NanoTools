from __future__ import print_function

import time
import os

from metis.File import MutableFile
from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.Utils import do_cmd

"""
Let's say we want to compute 3*2*10. I know the answer is 60, only because I
have calculated it using this example. Since this is an involved calculation,
we will make 3 text files containing the number 10. We will submit condor jobs,
one per text file, which will take the file, multiply the content by two, and
copy back an output file. When the jobs are complete, we will sum up the outputs.
"""

if __name__ == "__main__":

    # Make a base directory
    basedir = "/hadoop/cms/store/user/{0}/metis_test/example/".format(os.getenv("USER"))
    MutableFile(basedir).touch()

    # Make 3 text files (file_<i>.txt) in the base directory and fill them with text "10" 
    mfs = []
    for i in range(3):
        mf = MutableFile("{0}/file_{1}.txt".format(basedir,i))
        mf.rm()
        mf.append("10\n")
        mfs.append(mf)

    # Make a directory sample, giving it the location and a dataset name for bookkeeping purposes
    # The globber must be customized (by default, it is *.root) in order to pick up the text files
    ds = DirectorySample(location=basedir, dataset="/TEST/Examplev1/TEST", globber="*.txt")

    # Make the condor executable to be run on the worker node
    # Need to assume the argument mapping used by CondorTask
    # The executable just takes an input file and multiplies the content by two to produce the output
    exefile = MutableFile("dummy_exe.sh")
    exefile.rm()
    exefile.append(r"""#!/bin/bash
    OUTPUTDIR=$1
    OUTPUTNAME=$2
    INPUTFILENAMES=$3
    IFILE=$4
    # Make sure OUTPUTNAME doesn't have .root
    OUTPUTNAME=$(echo $OUTPUTNAME | sed 's/\.root//')
    echo $(( 2*$(cat $INPUTFILENAMES) )) > tmp.txt
    gfal-copy -p -f -t 4200 --verbose file://`pwd`/tmp.txt gsiftp://gftp.t2.ucsd.edu${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.txt --checksum ADLER32
    """)
    exefile.chmod("u+x")

    # Make a CondorTask (3 in total, one for each input)
    task = CondorTask(
            sample = ds,
            files_per_output = 1,
            tag = "v0",
            output_name = "output.txt",
            executable = exefile.get_name(),
            condor_submit_params = {"sites": "UAF,T2_US_UCSD,UCSB"},
            no_load_from_backup = True, # for the purpose of the example, don't use a backup
    )
    do_cmd("rm -rf {0}".format(task.get_outputdir()))


    # Process and sleep until complete
    is_complete = False
    for t in [5.0, 5.0, 10.0, 15.0, 20.0]:
        task.process()
        print("Sleeping for {0} seconds".format(int(t)))
        time.sleep(t)
        is_complete = task.complete()
        if is_complete: break

    # If it's complete, make a dummy sample out of the output directory
    # in order to pick up the files. Then cat out the contents and sum
    # them up. This should be 3*2*10 = 100
    if is_complete:
        print("Job completed! Checking outputs...")
        outsamp = DirectorySample(location=task.get_outputdir(), dataset="/Blah/blah/BLAH", globber="*.txt")
        tot = 0
        for f in outsamp.get_files():
            mf = MutableFile(f.get_name())
            tot += int(mf.cat())
        print("It looks like we found 3*2*10 = {0}".format(tot))

