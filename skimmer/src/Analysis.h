#ifndef ANALYSIS_H
#define ANALYSIS_H

// RAPIDO
#include "arbusto.h"
#include "cutflow.h"
#include "hepcli.h"
// ROOT
#include "TString.h"
// NanoCORE
#include "Nano.h"
#include "Config.h"             // gconf
#include "Tools/goodrun.h"

#include "TruthAnalysis.h"
#include "ObjectSelection_Leptons.h"
#include "ObjectSelection_Jets.h"
#include "SkimmerCutBase.h"


typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class Analysis
{
  public:

  // Memeber variables
    TList *runs;
    TList *lumis;
    Arbusto &arbusto;
    Nano &nt;
    HEPCLI &cli;
    Cutflow &cutflow;
    TruthAnalysis truthAna;
    LeptonSelection leptonSelection;
    JetSelection jetSelection;
    std::vector<Cut *> vCutflowCuts_;
    bool passCutflow_;
    std::string finalSkimmerCut_ = "TheEnd";

    // Constructor
    Analysis(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
        : arbusto(arbusto_ref),
          nt(nt_ref), cli(cli_ref),
          cutflow(cutflow_ref),
          truthAna(arbusto_ref, nt_ref, cli_ref, cutflow_ref),
          leptonSelection(arbusto_ref, nt_ref, cli_ref, cutflow_ref.globals),
          jetSelection(arbusto_ref, nt_ref, cli_ref, cutflow_ref.globals)
    {
      // Initialize TLists for metadata TTrees
      runs = new TList();
      lumis = new TList();
    }

  // Initialize branches to be added to output "Events" TTree
  virtual void initBranches()
  {
    if (cli.is_signal && cli.dump_truth) {
      truthAna.initTruthBranches();
    }
  }

  // Define global variables and cutflow to be run in event loop
  // Note: the variables are set in the event loop when performing the object selecton. 
  //       The cutflow has to be run after the appropriate object selection has been performed.

  virtual void initCutflow()
  {
    // Initialize variables needed in cutflow.
    cutflow.globals.newVar<LorentzVectors>("veto_lep_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("vvh_veto_lep_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("tight_lep_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("ak4jets_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("ak8jets_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("ak8jets_run2sel_p4s", {});
    cutflow.globals.newVar<double>("vvh_lep_pt_lead", -999);
    cutflow.globals.newVar<double>("vvh_lep_pt_sub", -999);
    cutflow.globals.newVar<double>("ht_ak8", -999);
    cutflow.globals.newVar<double>("ht_ak8_run2sel", -999);
    cutflow.globals.newVar<double>("ht_ak4", -999);
    cutflow.globals.newVar<int>("n_ak4jets", -999);
    cutflow.globals.newVar<int>("n_ak8jets", -999);
    cutflow.globals.newVar<int>("n_ak8jets", -999);
    cutflow.globals.newVar<int>("n_ak8jets_run2sel", -999);
    cutflow.globals.newVar<int>("n_vbsjet_pairs", -999);

    // First cut
    Cut *cut_base = new LambdaCut("AllEvents", [&]()
                                  { return true; });
    cutflow.setRoot(cut_base);

    // Last cut
    Cut *cut_last = new LambdaCut("TheEnd", [&]()
                                  { return true; });
    cutflow.insert(cut_base, cut_last, Right);

    // Analysis-dependent cutflow, added between first and last cut
    if (vCutflowCuts_.size() > 0)
    {
      cutflow.insert(cut_base, vCutflowCuts_.at(0), Right);
      for (size_t i = 1; i < vCutflowCuts_.size(); ++i)
      {
        cutflow.insert(vCutflowCuts_.at(i - 1), vCutflowCuts_.at(i), Right);
      }
    }

  }


// Initialize per TTree, before event loop.
virtual void
initPerTTree(TTree *ttree)
  {
    // Initialize arbusto
    arbusto.tfile->cd();
    arbusto.init(ttree);

    // Store metadata ttrees
    TTree *runtree = ((TTree *)ttree->GetCurrentFile()->Get("Runs"))->CloneTree();
    runs->Add(runtree);
    TTree *lumitree = ((TTree *)ttree->GetCurrentFile()->Get("LuminosityBlocks"))->CloneTree();
    lumis->Add(lumitree);


    // Load golden JSON files
    if (nt.isData())
    {
      switch (nt.year())
      {
      case 2016:
        set_goodrun_file(
            "data/golden_jsons/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON_formatted.txt");
        break;
      case 2017:
        set_goodrun_file(
            "data/golden_jsons/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON_formatted.txt");
        break;
      case 2018:
        set_goodrun_file(
            "data/golden_jsons/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON_formatted.txt");
        break;
      default:
        throw std::runtime_error("Core::Analysis - invalid year or none set");
        break;
      }
    }

    // Set config (e.g. year, isAPV)
    TString file_name = cli.input_tchain->GetCurrentFile()->GetName();
    gconf.GetConfigsFromDatasetName(file_name.Data()); 
  }

  // Run per event in the event loop
  virtual void runPerEvent()
  {

    // Reset branches and globals
    arbusto.resetBranches();
    cutflow.globals.resetVars();

    // Dump truth information
    if (cli.is_signal && cli.dump_truth)
    {
      truthAna.setTruthCandidates();
    }

    // Run lepton selection
    leptonSelection.selectVetoLeptons();

    // Run vvh lepton selection
    leptonSelection.selectVVHVetoLeptons();

    // Run jets selection
    jetSelection.selectJets();

    // Run cutflow
    std::vector<std::string> cuts_to_check = {finalSkimmerCut_};
    std::vector<bool> checkpoints = cutflow.run(cuts_to_check);
    passCutflow_ = checkpoints.at(0);
  }

  // Return skimming decision for a given event after running the cutflow
  virtual bool eventPassed(){
    return passCutflow_;
  }

  virtual bool passEventFilters(){
    bool passFilters = nt.Flag_goodVertices() &&
                       nt.Flag_HBHENoiseFilter() &&
                       nt.Flag_HBHENoiseIsoFilter() &&
                       nt.Flag_EcalDeadCellTriggerPrimitiveFilter() &&
                       nt.Flag_BadPFMuonFilter() &&
                       nt.Flag_BadPFMuonDzFilter() &&
                       nt.Flag_hfNoisyHitsFilter() &&
                       nt.Flag_eeBadScFilter() &&
                       ((nt.year() == 2016) || nt.Flag_ecalBadCalibFilter()) &&
                       ((!nt.isData()) || nt.Flag_globalSuperTightHalo2016Filter());
    return passFilters;
  }

  // Write output TTrees at the end of the job
  virtual void writeOutput()
  {
    TTree *merged_runs = TTree::MergeTrees(runs);
    merged_runs->SetName("Runs");
    TTree *merged_lumis = TTree::MergeTrees(lumis);
    merged_lumis->SetName("LuminosityBlocks");

    arbusto.tfile->cd();
    merged_runs->Write();
    merged_lumis->Write();
    arbusto.write();
  }

  virtual std::string finalSkimmerCut()
  {
    return finalSkimmerCut_;
  }
};
#endif
