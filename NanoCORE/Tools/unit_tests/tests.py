#!/usr/bin/env python

import unittest
import os
import ROOT as r

def vround(a,digits=4):
    if (type(a) in [tuple, list]):
        return [round(x, digits) for x in a]
    elif (type(a) in [float]):
        return round(a, digits)
    else:
        return a

class CORETest(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        filename = "root://redirector.t2.ucsd.edu///store/user/namin/nanoaod/TTJets_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18NanoAODv5-Nano1June2019_102X_upgrade2018_realistic_v19-v1/8E0C8306-DC0D-0548-BA7C-D0698140DF28.root"
        jecera = "Autumn18_V19_MC";

        print(">>> Starting unit tests with\n\tfile={}\n\tjecera={}".format(filename, jecera))

        print(">>> Making TChain")
        ch = r.TChain("Events")
        ch.Add(filename);
        print(">>> Loading shared library")
        thispath = os.path.abspath(__file__)
        corebase = os.path.join(thispath.rsplit("NanoCORE/",1)[0], "NanoCORE")
        r.gSystem.Load('{corebase}/NANO_CORE.so'.format(corebase=corebase))
        print(">>> Loading includes")
        for include in ["Nano.cc", "SSSelections.cc", "MetSelections.cc", "Config.cc"]:
            r.gInterpreter.ProcessLine('#include "{corebase}/{include}"'.format(corebase=corebase, include=include))

        print(">>> Making jet corrector")
        p = dict(corebase=corebase, jecera=jecera)
        vs = r.std.vector("string")()
        vs.push_back("{corebase}/Tools/jetcorr/data/{jecera}/{jecera}_L1FastJet_AK4PFchs.txt".format(**p))
        vs.push_back("{corebase}/Tools/jetcorr/data/{jecera}/{jecera}_L2Relative_AK4PFchs.txt".format(**p))
        vs.push_back("{corebase}/Tools/jetcorr/data/{jecera}/{jecera}_L3Absolute_AK4PFchs.txt".format(**p))
        from ROOT import makeJetCorrector
        jetcorr = makeJetCorrector(vs)

        print(">>> Initializing nanotree")
        from ROOT import nt
        nt.Init(ch)
        print(">>> Getting first event")
        nt.GetEntry(0)
        print(">>> Done initializing")

        # attach objects to be used later to class instances
        cls.ch = ch
        cls.nt = nt
        cls.jetcorr = jetcorr

    def test_jec_correction(self):
        self.jetcorr.setJetPtEtaARho(50., 1.1, 0.5, 25.)
        corr = float(self.jetcorr.getCorrection())
        self.assertEqual(vround(corr), vround(1.01693546772))

    def test_jec_subcorrections(self):
        self.jetcorr.setJetPtEtaARho(50., 1.1, 0.5, 25.)
        subcorrs = map(float, self.jetcorr.getSubCorrections())
        self.assertEqual(vround(subcorrs), vround([0.9395483732223511, 1.0169354677200317, 1.0169354677200317]))

    def test_infer_year(self):
        self.assertEqual(self.nt.year(), 2018)

    def test_infer_mc(self):
        self.assertEqual(self.nt.isData(), False)

    def test_t1met(self):
        from ROOT import getT1CHSMET
        t1pair = getT1CHSMET(self.jetcorr)
        self.assertEqual(vround(self.nt.RawMET_pt()), vround(33.71875))
        self.assertEqual(vround(self.nt.RawMET_phi()), vround(2.087890625))
        self.assertEqual(vround(self.nt.MET_pt()), vround(30.6464557648))
        self.assertEqual(vround(self.nt.MET_phi()), vround(2.0341796875))
        self.assertEqual(vround(t1pair.first), vround(30.6478996277))
        self.assertEqual(vround(t1pair.second), vround(2.03437948227))

    def test_met_filters(self):
        from ROOT import gconf, passesMETfilters
        gconf.year = self.nt.year()
        self.assertEqual(passesMETfilters(self.nt.isData()), True)

    def test_electrons(self):
        from ROOT import isGoodElectron
        pts = list(self.nt.Electron_pt())
        nElectron = len(pts)
        self.assertEqual(nElectron, 1)
        self.assertEqual(vround(pts), vround([43.71261215209961]))
        self.assertEqual(isGoodElectron(0), False)

if __name__ == "__main__":
    unittest.main(verbosity=2)