// NanoTools
#include "main.h"

int main(int argc, char** argv) 
{
    std::cout << "Entered skimmer" << std::endl;

    // CLI
    HEPCLI cli = HEPCLI(argc, argv);

    // Initialize Looper
    if (cli.debug) std::cout << "Build looper" << std::endl;
    Looper looper = Looper(cli.input_tchain);

    // Initialize Arbusto
    if (cli.debug) std::cout << "Create output file" << std::endl;
    TFile* output_tfile = new TFile(
        TString(cli.output_dir+"/"+cli.output_name+".root"),
        "RECREATE"
    );

    // Set to true to specify branches to DROP instead of keep
    bool remove_branches = false;

    // Output setting (setting which TBranches to save from original Nano)
    if (cli.debug) std::cout << "Initialize arbusto" << std::endl;
    Arbusto arbusto = Arbusto(
        output_tfile,
        cli.input_tchain,
        {
            "Electron*",
            "Muon*",
            "Jet*",
            "Tau*",
            "GenPart*",
            "Generator*",
            "FatJet*",
            "MET*",
            "event*",
            "run*",
            "luminosityBlock*",
            "genWeight*",
            "btagWeight*",
            "LHE*",
            "*Weight*",
            "Flag*",
            "SubJet*",
            "HLT_*",
            "Pileup*"
        },
        remove_branches
    );

    arbusto.newVecBranch<float>("Electron_mvaTTHUL", {});
    arbusto.newVecBranch<float>("Muon_mvaTTHUL", {});
    arbusto.newBranch<float>("Pass_leptonVeto", {});
    MVATTH::MVATTH mvatth("./data/leptonMVA/UL20_2018.xml");

    // Make sure HLT branches for all years are present. 
    // This avoids missing branches in analysis code.
    // Note: if the branch exists, the leaf value is reset in the event loop.
    std::vector<TString> branches = {"HLT_PFHT1050","HLT_PFHT800", "HLT_PFHT900"}; 
    for (auto &branch : branches){ 
      std::cout << "Adding branch " << branch << std::endl;
      arbusto.newBranch<bool>(branch, false);
    }


    // Initialize TLists for metadata TTrees
    TList* runs = new TList();
    TList* lumis = new TList();
    
    std::vector<TString> missingBranches;
    int counter_passAllHad {0};

    // Run looper
    tqdm bar;
    if (cli.debug) std::cout << "Start looper" << std::endl;
    looper.run(

        // Lambda function called once per TTree
        [&](TTree* ttree)
        {
            if (cli.debug) std::cout << "Initialize NanoTools" << std::endl;
            nt.Init(ttree);

            arbusto.tfile->cd();
            
            if (cli.debug) std::cout << "Get file-level trees" << std::endl;
            // Store metadata ttrees
            TTree* runtree = ((TTree*)ttree->GetCurrentFile()->Get("Runs"))->CloneTree();
            runs->Add(runtree);
            TTree* lumitree = ((TTree*)ttree->GetCurrentFile()->Get("LuminosityBlocks"))->CloneTree();
            lumis->Add(lumitree);

            if (cli.debug) std::cout << "Initialize arbusto" << std::endl;
            arbusto.init(ttree);
            TString file_name = cli.input_tchain->GetCurrentFile()->GetName();
            gconf.GetConfigs(nt.year());
            gconf.isAPV = (file_name.Contains("HIPM_UL2016") || file_name.Contains("NanoAODAPV") || file_name.Contains("UL16APV"));
        },

        // Lambda function called once per Entry in a TTree
        [&](int entry) 
        {   
           
            // if this is a debug run end the loop after 10000
            if (cli.debug && looper.n_events_processed == 10000)
            {
                std::cout << "entered" << std::endl;
                looper.stop();
            }
            // Otherwise process the event
            else
            {
              // Reset branches and globals
              arbusto.resetBranches();
              // Load event information
              nt.GetEntry(entry);
              // progess bar printing
              bar.progress(looper.n_events_processed, looper.n_events_total);

              // ==========================================
              // Do your stuff
              // If "return" is called, the event will not be saved (as it won't reach the line that says arbusto.fill(entry);            

              // Do not store events with incomplete Madgraph information 
              if( nt.nLHEReweightingWeight()<=0){ return; }

              //
              for (unsigned int elec_i = 0; elec_i < nt.nElectron(); ++elec_i)
              {
                float tth_mva = mvatth.computeElecMVA(elec_i);
                arbusto.appendToVecLeaf<float>("Electron_mvaTTHUL", tth_mva);
                }
                for (unsigned int muon_i = 0; muon_i < nt.nMuon(); ++muon_i)
                {
                    float tth_mva = nt.Muon_mvaTTH().at(muon_i);
                    arbusto.appendToVecLeaf<float>("Muon_mvaTTHUL", tth_mva);
                }

                bool pass_leptonVeto = passLeptonVeto(nt);
                arbusto.setLeaf<bool>("Pass_leptonVeto", pass_leptonVeto);
                if (!pass_leptonVeto){ return; }

                bool pass_jetsSelection = runJetsSelection_Run2(nt);
                if (!pass_jetsSelection){ return; }
                 
              // ==========================================

              // If it reaches here then save the event
                counter_passAllHad++;
                arbusto.fill(entry);
            }
        }
    );

    std::cout << "looper.n_events_processed : " << looper.n_events_processed << std::endl;
    std::cout << "Events that passed allHad : " << counter_passAllHad << std::endl;

    TTree* merged_runs = TTree::MergeTrees(runs);
    merged_runs->SetName("Runs");
    TTree* merged_lumis = TTree::MergeTrees(lumis);
    merged_lumis->SetName("LuminosityBlocks");

    arbusto.tfile->cd();
    merged_runs->Write();
    merged_lumis->Write();
    arbusto.write();
    std::cout << "Exit"  << std::endl;
    return 0;
}
