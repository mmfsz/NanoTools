#!/bin/env python

import glob
import sys
import os
import ROOT as r
import json
import io

xsecs = {
    "DYto2L-2Jets_MLL-50_TuneCP5_13p6TeV_amcatnloFXFX-pythia8" : 6680.0,
    "TTto2L2Nu_TuneCP5_13p6TeV_powheg-pythia8" : 98.88,
    "WW_TuneCP5_13p6TeV_pythia8" : 80.23,
    "WtoLNu-2Jets_TuneCP5_13p6TeV_amcatnloFXFX-pythia8" : 67710.0,
    }

try:
    to_unicode = unicode
except NameError:
    to_unicode = str

# The output directory will be assumed to be in the format of:
# NOTE: the "TAG" location
# /ceph/cms/store/user/USER/some/dir_TAG/SAMPLE_TuneCP5_BLAHBLAH_TAG/output_1.root
# /ceph/cms/store/user/USER/some/dir_TAG/SAMPLE_TuneCP5_BLAHBLAH_TAG/...
# /ceph/cms/store/user/USER/some/dir_TAG/SAMPLE_TuneCP5_BLAHBLAH_TAG/output_N.root

def create_empty_dict():
    sd = {}
    sd["xsec"] = 0.0
    sd["year"] = ""
    sd["treeName"] = "Events"
    sd["histAxisName"] = ""
    sd["options"] = ""
    sd["WCnames"] = []
    sd["files"] = []
    sd["nEvents"] = 0
    sd["nGenEvents"] = 0.0
    sd["nSumOfWeights"] = 0.0
    sd["isData"] = False
    sd["path"] = ""
    return sd

def parse_year(sample):
    if "RunIIAutumn18NanoAOD" in sample: year_ = "2018";
    elif "RunIIFall17NanoAOD" in sample: year_ = "2017";
    elif "RunIISummer16NanoAOD" in sample: year_ = "2016";
    elif "Run3Summer22" in sample: year_ = "2022";
    # Ultra-legacy
    elif "RunIISummer20UL18" in sample: year_ = "2018";
    elif "RunIISummer20UL17" in sample: year_ = "2017";
    elif "RunIISummer20UL16" in sample: year_ = "2016";
    # Ultra-legacy
    elif "RunIISummer19UL18" in sample: year_ = "2018";
    elif "RunIISummer19UL17" in sample: year_ = "2017";
    elif "RunIISummer19UL16" in sample: year_ = "2016";
    # Data
    elif "Run2018" in sample: year_ = "2018";
    elif "Run2017" in sample: year_ = "2017";
    elif "Run2016" in sample: year_ = "2016";
    elif "Run2022" in sample: year_ = "2022";
    return year_

def isAPV(sample):
    if "HIPM_UL2016" in sample: return True
    elif "NanoAODAPV" in sample: return True
    elif "UL16APV" in sample: return True
    return False

def isdata(sample):
    bn = os.path.basename(sample)
    if "NANOAOD_" in bn:
        return True
    else:
        return False

def parse_short_name(sample):
    if isdata(sample):
        bn = os.path.basename(sample)
        sn = bn.split("_NANOAOD")[0]
    else:
        bn = os.path.basename(sample)
        sn = bn.split("_Run")[0]
    return sn

def parse_xsec(sample):
    if isdata(sample):
        return 0.0
    else:
        return xsecs[parse_short_name(sample)]


cephdir = os.path.normpath(sys.argv[1])
tag = os.path.basename(cephdir)
samples = glob.glob("{}/*_{}".format(cephdir, tag))

os.system("mkdir -p {}/jsons".format(cephdir))

for sample in samples:

    print("Creating json for sample = {}".format(sample))

    rootfiles = glob.glob("{}/*.root".format(sample))
    rootfiles.sort()

    sd = create_empty_dict()
    sd["year"] = parse_year(sample) + ("APV" if isAPV(sample) else "")
    sd["histAxisName"] = parse_short_name(sample)
    sd["files"] = rootfiles
    sd["isData"] = isdata(sample)
    sd["path"] = sample
    sd["xsec"] = parse_xsec(sample)

    nEvents = 0
    nGenEvents = 0
    nSumOfWeights = 0
    for rootfile in rootfiles:
        f = r.TFile(rootfile)
        t = f.Get(sd["treeName"])
        nEvents += t.GetEntries()
        rt = f.Get("Runs")
        for run in rt:
            if not sd["isData"]:
                nGenEvents = run.genEventCount
                nSumOfWeights = run.genEventSumw
            else:
                nGenEvents = 0
                nSumOfWeights = 0.0

    sd["nEvents"] = nEvents
    sd["nGenEvents"] = nGenEvents
    sd["nSumOfWeights"] = nSumOfWeights

    with io.open("{}/jsons/{}.json".format(cephdir, sd["histAxisName"]), "w") as f:
        str_to_write = json.dumps(sd, indent=4)
        f.write(to_unicode(str_to_write))
