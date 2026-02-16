from metis.Sample import DirectorySample, DBSSample
from vbsvvh_data import nanoaodv9_data
from vbsvvh_mc import nanoaodv9_bkg, nanoaodv9_test, nanoaodv15_run2_bkg


# Master list of all samples
# Specify a dataset name and a short name for the output root file on nfs

samples_to_submit = []

#samples_to_submit += nanoaodv9_bkg
#samples_to_submit += nanoaodv9_data
samples_to_submit += nanoaodv15_run2_bkg

print(samples_to_submit)
