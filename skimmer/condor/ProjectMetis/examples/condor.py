from __future__ import print_function

from pprint import pprint

from metis.Utils import condor_q, get_hist

import math, sys, os

"""
Some simple uses of the condor_q API
"""


if __name__ == "__main__":

    # Do condor_q and print out a dict for the first job
    # By default, only shows jobs for $USER
    my_jobs = condor_q()
    if my_jobs:
        print("-- my jobs --")
        pprint(my_jobs[0])
        print()

    # Don't specify a user, so get all jobs
    all_jobs = condor_q(user="")
    if all_jobs:
        print("-- all jobs --")
        pprint(all_jobs[0])
        print()

    # Get all job statuses and print out a counts of the
    # different statuses
    all_statuses = [job["JobStatus"] for job in all_jobs]
    if all_statuses:
        print(get_hist(all_statuses))
