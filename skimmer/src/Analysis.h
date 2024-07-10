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
    bool passCutflow_;

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

    arbusto.newVecBranch<float>("Electron_mvaTTHUL", {});
    arbusto.newVecBranch<float>("Muon_mvaTTHUL", {});
    arbusto.newVecBranch<int>("veto_lep_p4s", {});
    arbusto.newVecBranch<int>("veto_lep_idxs", {});
    arbusto.newVecBranch<int>("veto_lep_jet_idxs", {});
    arbusto.newVecBranch<int>("veto_lep_pdgIDs", {});
  }

  // Define global variables and cutflow to be run in event loop
  // Note: the variables are set in the event loop when performing the object selecton. 
  //       The cutflow has to be run after the appropriate object selection has been performed.

  virtual void initCutflow()
  {
    // Initialize variables needed in cutflow.
    cutflow.globals.newVar<LorentzVectors>("veto_lep_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("tight_lep_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("ak4jets_p4s", {});
    cutflow.globals.newVar<LorentzVectors>("ak8jets_p4s", {});
    cutflow.globals.newVar<double>("ht_ak8", -999);
    cutflow.globals.newVar<double>("ht_ak4", -999);
    cutflow.globals.newVar<int>("n_ak4jets", -999);
    cutflow.globals.newVar<int>("n_ak8jets", -999);
    cutflow.globals.newVar<int>("n_vbsjet_pairs", -999);

    // All events
    Cut *cut_base = new LambdaCut("AllEvents", [&]()
                                  { return true; });
    cutflow.setRoot(cut_base);

    // Is good run number (data only)
    Cut *cut_isGoodDataRun = new LambdaCut("isGoodDataRun", [&]()
                                           { return (nt.isData()) ? goodrun(nt.run(), nt.luminosityBlock()) : true; });
    cutflow.insert(cut_base, cut_isGoodDataRun, Right);

    // Pass Event filters 
    Cut *cut_passEventFilters = new LambdaCut("PassEventFilters", [&]()
                                              { return passEventFilters(); });
    cutflow.insert(cut_isGoodDataRun, cut_passEventFilters, Right);

    // Lepton selection
    Cut *cut_noVetoLeps = new LambdaCut(
        "NoVetoLeptons",
        [&]()
        {
          return (cutflow.globals.getVal<LorentzVectors>("veto_lep_p4s").size() == 0);
        });
    cutflow.insert(cut_passEventFilters, cut_noVetoLeps, Right);

    // Jet selection
    Cut *cut_AtLeast2AK8Jets = new LambdaCut(
        "AtLeast2AK8Jets",
        [&]()
        {
          return (cutflow.globals.getVal<int>("n_ak8jets") >= 2);
        });
    cutflow.insert(cut_noVetoLeps, cut_AtLeast2AK8Jets, Right);

    Cut *cut_AK8HTgt1100 = new LambdaCut(
        "AK8HTgt1100",
        [&]()
        {
          return (cutflow.globals.getVal<double>("ht_ak8") > 1100);
        });
    cutflow.insert(cut_AtLeast2AK8Jets, cut_AK8HTgt1100, Right);

    // The end 
    Cut *cut_last = new LambdaCut("TheEnd", [&]()
                                  { return true; });
    cutflow.insert(cut_AK8HTgt1100, cut_last, Right);
  }

  // Initialize per TTree, before event loop.
  virtual void initPerTTree(TTree *ttree)
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

  virtual void runPerEvent(){

      // Dump truth information
      if (cli.is_signal && cli.dump_truth)
      {
        truthAna.setTruthCandidates(); 
      }

      // Run lepton selection
      leptonSelection.selectVetoLeptons();

      // Run jets selection
      jetSelection.selectJets();

      // Run cutflow
      std::vector<std::string> cuts_to_check = {"TheEnd"};
      std::vector<bool> checkpoints = cutflow.run(cuts_to_check);
      passCutflow_ = checkpoints.at(0);
  }

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

};

#endif