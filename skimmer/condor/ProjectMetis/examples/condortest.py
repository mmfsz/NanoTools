import metis.Utils as Utils
from metis.CondorTask import CondorTask
from metis.Sample import FilelistSample

with open("condortest_exe.sh", "w") as fh:
    fh.write("""#!/bin/bash\necho "hello from $(hostname) at $(date)" """)

dummy = CondorTask(
        sample = FilelistSample(
            filelist = ["input_1.root"],
            dataset = "/test/test/TEST",
            ),
        files_per_output = 1,
        executable = "./condortest_exe.sh",
        tag = "v0",
        condor_submit_params = {
            "sites": "UAF",
            "requirements_line": "Requirements = ",
            },
        no_load_from_backup = True,
        )
dummy.process()
