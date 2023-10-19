// NanoTools
#include "main.h"

int main(int argc, char** argv) 
{

    // CLI
    HEPCLI cli = HEPCLI(argc, argv);

    // Initialize Looper
    Looper looper = Looper(cli.input_tchain);

    // Initialize Arbusto
    TFile* output_tfile = new TFile(
        TString(cli.output_dir+"/"+cli.output_name+".root"),
        "RECREATE"
    );

    // Set to true to specify branches to DROP instead of keep
    bool remove = true;

    // Output setting (setting which TBranches to save from original Nano)
    Arbusto arbusto = Arbusto(
        output_tfile,
        cli.input_tchain,
        {
            "Jet*"
        },
        remove
    );

    // Initialize TLists for metadata TTrees
    TList* runs = new TList();
    TList* lumis = new TList();

    // Run looper
    tqdm bar;
    looper.run(

        // Lambda function called once per TTree
        [&](TTree* ttree)
        {
            nt.Init(ttree);

            // Store metadata ttrees
            TTree* runtree = ((TTree*)ttree->GetCurrentFile()->Get("Runs"))->CloneTree();
            runtree->SetDirectory(0);
            runs->Add(runtree);
            TTree* lumitree = ((TTree*)ttree->GetCurrentFile()->Get("LuminosityBlocks"))->CloneTree();
            lumitree->SetDirectory(0);
            lumis->Add(lumitree);

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

                // If there aren't even two leptons that skip
                if (not (nt.nElectron() + nt.nMuon() >= 2)) return;

                // Counter counting nveto leptons
                int nvetolep = 0;
                int nvetoelec = 0;
                int nvetomuon = 0;

                // Loop over the electrons and count passing "veto"
                for (unsigned int iel = 0; iel < nt.nElectron(); ++iel)
                {
                    if (WWZ::electronID(iel, WWZ::IDveto, nt.year()))
                        nvetoelec++;
                }

                // Loop over the muons and count passing "veto"
                for (unsigned int imu = 0; imu < nt.nMuon(); ++imu)
                {
                    if (WWZ::muonID(imu, WWZ::IDveto, nt.year()))
                        nvetomuon++;
                }

                nvetolep = nvetoelec + nvetomuon;

                if (not (nvetolep >= 2)) return;
                // ==========================================

                // If it reaches here then save the event
                arbusto.fill(entry);
            }
        }
    );


    TTree* merged_runs = TTree::MergeTrees(runs);
    merged_runs->SetName("Runs");
    TTree* merged_lumis = TTree::MergeTrees(lumis);
    merged_lumis->SetName("LuminosityBlocks");

    arbusto.tfile->cd();
    merged_runs->Write();
    merged_lumis->Write();
    arbusto.write();
    return 0;
}
