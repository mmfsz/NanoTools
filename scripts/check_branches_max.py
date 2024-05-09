#!/bin/env python3

# Script to check max values in branches. These are hardcoded in NanoCore/Nano.h
# Ex: #define NLHEREWEIGHTINGWEIGHT_MAX 200 // for LHEReweightingWeight_* collection

import ROOT as r
import sys

f = r.TFile(sys.argv[1])
t = f.Get("Events")

print("nboostedTau", t.GetMaximum("nboostedTau"))
print("nCorrT1METJet", t.GetMaximum("nCorrT1METJet"))
print("nElectron", t.GetMaximum("nElectron"))
print("nFatJet", t.GetMaximum("nFatJet"))
print("nFsrPhoton", t.GetMaximum("nFsrPhoton"))
print("nGenJetAK8", t.GetMaximum("nGenJetAK8"))
print("nGenJet", t.GetMaximum("nGenJet"))
print("nGenPart", t.GetMaximum("nGenPart"))
print("nSubGenJetAK8", t.GetMaximum("nSubGenJetAK8"))
print("nGenVisTau", t.GetMaximum("nGenVisTau"))
print("nLHEPdfWeight", t.GetMaximum("nLHEPdfWeight"))
print("nLHEReweightingWeight", t.GetMaximum("nLHEReweightingWeight"))
print("nLHEScaleWeight", t.GetMaximum("nLHEScaleWeight"))
print("nPSWeight", t.GetMaximum("nPSWeight"))
print("nIsoTrack", t.GetMaximum("nIsoTrack"))
print("nJet", t.GetMaximum("nJet"))
print("nLHEPart", t.GetMaximum("nLHEPart"))
print("nLowPtElectron", t.GetMaximum("nLowPtElectron"))
print("nMuon", t.GetMaximum("nMuon"))
print("nPhoton", t.GetMaximum("nPhoton"))
print("nGenDressedLepton", t.GetMaximum("nGenDressedLepton"))
print("nGenIsolatedPhoton", t.GetMaximum("nGenIsolatedPhoton"))
print("nSoftActivityJet", t.GetMaximum("nSoftActivityJet"))
print("nSubJet", t.GetMaximum("nSubJet"))
print("nTau", t.GetMaximum("nTau"))
print("nTrigObj", t.GetMaximum("nTrigObj"))
print("nOtherPV", t.GetMaximum("nOtherPV"))
print("nSV", t.GetMaximum("nSV"))
