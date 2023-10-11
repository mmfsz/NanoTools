import time
import ROOT as r
import argparse
import sys

parser = argparse.ArgumentParser()
parser.add_argument('inputs', help='comma-separated file names')
parser.add_argument("-o", '--output', default="output.root", help="output file name")
args = parser.parse_args()

ch = r.TChain("Events")
for fname in args.inputs.split(","):
    fname = fname.strip()
    if fname.startswith("/store/"):
        fname = "root://xcache-redirector.t2.ucsd.edu:2042/" + fname
    ch.Add(fname)

ch.SetBranchStatus("*", 0)
ch.SetBranchStatus("Jet_pt", 1)

fout = r.TFile(args.output, "recreate")
h = r.TH1F("ht", "ht", 500, 0, 1000)

N = ch.GetEntries()
print("Looping on TChain with {} entries".format(N))
t0 = time.time()
for i, evt in enumerate(ch):
    if (i == N-1) or (i % 20000 == 0):
        print("Reached entry {} in {:.2f}s".format(i, time.time()-t0))
        sys.stdout.flush()
    pts = list(evt.Jet_pt)
    ht = sum(pt for pt in pts if pt > 40.)
    h.Fill(ht)

h.Write()
fout.Close()
