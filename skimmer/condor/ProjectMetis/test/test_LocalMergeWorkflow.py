import os
import glob
import time
import unittest

from metis.LocalMergeTask import LocalMergeTask
from metis.Utils import do_cmd
from metis.Path import Path
from metis.File import File, MutableFile

from pprint import pprint


class LocalMergeWorkflowTest(unittest.TestCase):

    @unittest.skipIf(os.getenv("FAST"), "Skipped due to impatience")
    @unittest.skipIf("uaf-" not in os.uname()[1], "ROOT only on UAF")
    def test_workflow(self):

        import ROOT as r

        basepath = "/tmp/{}/metis/localmerge/".format(os.getenv("USER"))

        # Make the base directory
        MutableFile(basepath).touch()

        # Clean up before running
        do_cmd("rm {}/*.root".format(basepath))

        for i in range(0,3):
            f = r.TFile("{}/in_{}.root".format(basepath,i),"RECREATE")
            h = r.TH1F()
            h.Write()
            f.Close()

        outname = "/home/users/namin/2017/test/ProjectMetis/testout/out.root"
        task = LocalMergeTask(
                # input_filenames=glob.glob("/hadoop/cms/store/user/namin/AutoTwopler_babies/FT_v1.06_v2/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/output/output_4*.root"),
                input_filenames=glob.glob(basepath+"/in_*.root"),
                output_filename=basepath+"/out.root",
                )

        task.process()

        self.assertEqual( task.get_outputs()[0].exists(), True )

if __name__ == "__main__":
    unittest.main()
