{

    gROOT->ProcessLine(".L ../NanoCORE/NANO_CORE.so");
    gROOT->ProcessLine(".L ScanChain_ss.C+");
    TChain *ch = new TChain("Events");

    ch->Add("root://redirector.t2.ucsd.edu///store/user/namin/nanoaod/TTJets_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18NanoAODv5-Nano1June2019_102X_upgrade2018_realistic_v19-v1/8E0C8306-DC0D-0548-BA7C-D0698140DF28.root");

    ScanChain(ch);

}
