// NanoTools
#include "main.h"
#include <chrono>
#include <TChain.h>
#include <TObjArray.h>
#include <TObjString.h>

int main(int argc, char **argv)
{
  std::cout << "--> Entered skimmer" << std::endl;
  auto start = std::chrono::high_resolution_clock::now();

  // CLI
  HEPCLI cli = HEPCLI(argc, argv);
  if (cli.debug) { std::cout << "--> Initialized HEPCLI " << std::endl; }

  // Initialize Looper
  Looper looper = Looper(cli.input_tchain);
  if (cli.debug) { std::cout << "--> Initialized looper " << std::endl; }

  // Initialize Arbusto
  TFile *output_tfile = new TFile(TString(cli.output_dir + "/" + cli.output_name + ".root"), "RECREATE");
  // Set to true to specify branches to DROP instead of keep
  bool remove_branches = false;

  // Output setting (setting which TBranches to save from original Nano)
  Arbusto arbusto = Arbusto(
      output_tfile,
      cli.input_tchain,
      {"Electron*",
       "Muon*",
       "Jet*",
       "Tau*",
       "GenPart*",
       "GenJet*",
       "Generator*",
       "FatJet*",
       "*MET*",
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
  if (cli.debug) { std::cout << "--> Initialize arbusto" << std::endl; }

  // Initialize Cutflow
  Cutflow cutflow = Cutflow(cli.output_name + "_Cutflow");
  
  // Initialize base Analysis class object (also adds branches)
  std::unique_ptr<Analysis> skimmer;

  std::cout << "--> Running analyzer: " << cli.analysis_tag << std::endl;
  if(cli.analysis_tag == "AllHadRun2") {
    skimmer = std::make_unique<Analysis_AllHadRun2>(arbusto, nt, cli, cutflow);
  }
  else if(cli.analysis_tag == "AllHad") {
    skimmer = std::make_unique<Analysis_AllHad>(arbusto, nt, cli, cutflow);
  }
  else if(cli.analysis_tag == "SemiMergHad") {
    skimmer = std::make_unique<Analysis_SemiMergHad>(arbusto, nt, cli, cutflow);
  }
  else if (cli.analysis_tag == "None")
  {
    skimmer = std::make_unique<Analysis>(arbusto, nt, cli, cutflow);
  }
  else if (cli.analysis_tag == "LepVeto")
  {
    skimmer = std::make_unique<Analysis_LepVeto>(arbusto, nt, cli, cutflow);
  }
  else if (cli.analysis_tag == "LepVeto_2AK8")
  {
    skimmer = std::make_unique<Analysis_LepVeto_2AK8>(arbusto, nt, cli, cutflow);
  }
  else{
    throw std::runtime_error("Error: Did not recognize analysis_tag.");
  }


  // Initialize branches to be added 
  skimmer->initBranches();

  // Define cutflow (event selection cuts)
  skimmer->initCutflow();

  std::vector<TString> missingBranches; // FIXME
  int counter_passAllHad{0};

  // -------------------------------------
  // Run looper

  tqdm bar;
  if (cli.debug) { std::cout << "--> Start looper" << std::endl; }

  looper.run(

      // Lambda function called once per TTree
      [&](TTree *ttree)
      {

        // Initialize NanoTools
        nt.Init(ttree);

        // Initialize info that changes only per TTree
        skimmer->initPerTTree(ttree);

      },

      // Lambda function called once per Entry in a TTree
      [&](int entry)
      {
        // if this is a debug run end the loop after 1000
        if (cli.debug && looper.n_events_processed == 1000)
        {
          looper.stop();
        }
        // Otherwise process the event
        else
        {
   

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

          // Run object reconstruction and event selection
          skimmer->runPerEvent();

          // Only store events that passed the selection
          if (!skimmer->eventPassed())
          {
            return;
          }

          // ==========================================

          // If it reaches here then save the event
          counter_passAllHad++;
          arbusto.fill(entry);
        }
      });

  std::cout << "--> Write output files" << std::endl;
  skimmer->writeOutput();

  cutflow.write(cli.output_dir);
  cutflow.writeCSV(cli.output_dir);

  // Save the original buffer
  std::streambuf *originalCoutBuffer = std::cout.rdbuf();
  std::ofstream outCutflowFile(cli.output_dir + "/cutflow.txt");
  // Redirect std::cout to the file
  std::cout.rdbuf(outCutflowFile.rdbuf());
  // Print sample information
  TObjArray *fileElements = cli.input_tchain->GetListOfFiles();
  int counter{1};
  for (auto *element : *fileElements)
  {
    const char *current_file_name = element->GetTitle();
    std::cout << "File " << counter << " : " << current_file_name << std::endl;
    counter++;
  }
  // Print analyzer that produced the cutflow
  std::cout << std::endl << "Analyzer: " << cli.analysis_tag << std::endl;
  // Print the cutflow
  cutflow.print();

  std::cout << "--> Events processed : " << looper.n_events_processed << std::endl;
  std::cout << "--> Events that passed " << skimmer->finalSkimmerCut() << " : " << counter_passAllHad << std::endl;

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;

  // Restore the original buffer
  std::cout.rdbuf(originalCoutBuffer);
  // Close the file
  outCutflowFile.close();
  
  return 0;
}
