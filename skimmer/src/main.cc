// NanoTools
#include "main.h"
#include <chrono>

int main(int argc, char **argv)
{
  std::cout << "Entered skimmer" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  // CLI
  HEPCLI cli = HEPCLI(argc, argv);

  // Initialize Looper
  if (cli.debug)
    std::cout << "Build looper" << std::endl;
  Looper looper = Looper(cli.input_tchain);

  // Initialize Arbusto
  if (cli.debug)
    std::cout << "Create output file" << std::endl;
  TFile *output_tfile = new TFile(TString(cli.output_dir + "/" + cli.output_name + ".root"), "RECREATE");

  // Set to true to specify branches to DROP instead of keep
  bool remove_branches = false;

  // Output setting (setting which TBranches to save from original Nano)
  if (cli.debug)
    std::cout << "Initialize arbusto" << std::endl;
  Arbusto arbusto = Arbusto(
      output_tfile,
      cli.input_tchain,
      {"Electron*",
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
       "Pileup*"},
      remove_branches);

  // Initialize Cutflow
  Cutflow cutflow = Cutflow(cli.output_name + "_Cutflow");
  
  // Initialize Analysis class object  (also adds branches)
  Analysis skimmer = Analysis(arbusto, nt, cli, cutflow);
  skimmer.initBranches();
  skimmer.initCutflow();

  // -------------------------------------

  // Initialize TLists for metadata TTrees
  TList *runs = new TList();
  TList *lumis = new TList();

  std::vector<TString> missingBranches;
  int counter_passAllHad{0};

  // Run looper
  tqdm bar;
  if (cli.debug)
    std::cout << "Start looper" << std::endl;
  looper.run(

      // Lambda function called once per TTree
      [&](TTree *ttree)
      {
        if (cli.debug)
          std::cout << "Initialize once per TTRee" << std::endl;

        nt.Init(ttree);
        arbusto.tfile->cd();
        // Store metadata ttrees
        TTree *runtree = ((TTree *)ttree->GetCurrentFile()->Get("Runs"))->CloneTree();
        runs->Add(runtree);
        TTree *lumitree = ((TTree *)ttree->GetCurrentFile()->Get("LuminosityBlocks"))->CloneTree();
        lumis->Add(lumitree);

        arbusto.init(ttree);
        skimmer.initPerTTree();
      },

      // Lambda function called once per Entry in a TTree
      [&](int entry)
      {
        // if this is a debug run end the loop after 10000
        if (cli.debug && looper.n_events_processed == 100)
        {
          looper.stop();
        }
        // Otherwise process the event
        else
        {
          // Reset branches and globals
          arbusto.resetBranches();
          cutflow.globals.resetVars();

          // Load event information
          nt.GetEntry(entry);
          // progess bar printing
          bar.progress(looper.n_events_processed, looper.n_events_total);

          // ==========================================
          // Do your stuff
          // If "return" is called, the event will not be saved (as it won't reach the line that says arbusto.fill(entry);

          // Step not required after haddnano.py was edited to remove empty files 
          // >>
          // Do not store events with incomplete Madgraph information 
          // Currently assumes private MC production name: it checks if dsname containts "VBSCuts" and "privateMC"
          // Edit in NanoCore/config.cc to work on central production
          // if (gconf.is_signal){
          //   if (nt.nLHEReweightingWeight() <= 0)
          //   {
          //     return;
          //   }
          // }
          // <<

          skimmer.runPerEvent();


          // bool pass_jetsSelection = runJetsSelection_Run2(nt);
          // bool pass_jetsSelection = runJetsSelection(nt);
          // if (!pass_jetsSelection)
          // {
          //   return;
          // }

          // bool passed = cutflow.run("PassEventFilters");
          if (!skimmer.eventPassed())
          {
            return;
          }

          // ==========================================

          // If it reaches here then save the event
          counter_passAllHad++;
          arbusto.fill(entry);
        }
      });

  
  cutflow.print();

  std::cout << "looper.n_events_processed : " << looper.n_events_processed << std::endl;
  std::cout << "Events that passed allHad : " << counter_passAllHad << std::endl;

  TTree *merged_runs = TTree::MergeTrees(runs);
  merged_runs->SetName("Runs");
  TTree *merged_lumis = TTree::MergeTrees(lumis);
  merged_lumis->SetName("LuminosityBlocks");

  arbusto.tfile->cd();
  merged_runs->Write();
  merged_lumis->Write();
  arbusto.write();
  std::cout << "Exit" << std::endl;
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
  return 0;
}
