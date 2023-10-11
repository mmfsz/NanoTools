import os
import time
import unittest

from metis.DummyTask import DummyMoveTask
from metis.Path import Path
from metis.File import File, MutableFile
from metis.Utils import do_cmd


class DummyMoveWorkflowTest(unittest.TestCase):

    def test_workflow(self):

        basepath = "/tmp/{}/metis/".format(os.getenv("USER"))

        # Clean up before running
        do_cmd("rm {}/*.root".format(basepath))

        # Make the base directory
        MutableFile(basepath).touch()

        # Set up 4 layers of input->output files
        step0, step1, step2, step3 = [], [], [], []
        for i in range(3):
            step0.append( MutableFile(name="{}/step0_{}.root".format(basepath,i)) )
            step1.append( MutableFile(name="{}/step1_{}.root".format(basepath,i)) )
            step2.append( MutableFile(name="{}/step2_{}.root".format(basepath,i)) )
            step3.append( MutableFile(name="{}/step3_{}.root".format(basepath,i)) )

        # Touch the step0 files to ensure they "exist", but they're still empty
        list(map(lambda x: x.touch(), step0))

        # Make a DummyMoveTask with previous inputs, outputs
        # each input will be moved to the corresponding output file
        # by default, completion fraction must be 1.0, but can be specified
        t1 = DummyMoveTask(
                inputs = step0,
                outputs = step1,
                # min_completion_fraction = 0.6,
                )

        # Clone first task for subsequent steps
        t2 = t1.clone(inputs = step1, outputs = step2)
        t3 = t1.clone(inputs = step2, outputs = step3)

        # Make a path, which will run tasks in sequence provided previous tasks
        # finish. Default dependency graph ("scheduled mode") will make it so 
        # that t2 depends on t1 and t3 depends on t1
        pa = Path([t1,t2])
        pb = Path([t3])

        # Yes, it was silly to make two paths, but that was done to showcase
        # the following concatenation ability (note that "addition" here is not
        # commutative)
        p1 = pa+pb

        while not p1.complete():
            p1.process()

            time.sleep(0.02)

        self.assertEqual(p1.complete(), True)


if __name__ == "__main__":
    unittest.main()
