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
#include "AnalysisSelection_Leptons.h"
#include "SkimmerCutBase.h"


typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class Analysis
{
  public:

  Arbusto &arbusto;
  Nano &nt;
  HEPCLI &cli;
  Cutflow &cutflow;
  TruthAnalysis truthAna;
  LeptonSelection leptonSelection;
  bool passCutflow_;

  Analysis(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
      : arbusto(arbusto_ref),
        nt(nt_ref), cli(cli_ref),
        cutflow(cutflow_ref),
        truthAna(arbusto_ref, nt_ref, cli_ref, cutflow_ref),
       leptonSelection(arbusto_ref, nt_ref, cli_ref, cutflow_ref.globals)
  {

  }

  virtual void initBranches()
  {
    if (cli.is_signal && cli.dump_truth) {
      truthAna.initTruthBranches();
    }

    arbusto.newVecBranch<float>("Electron_mvaTTHUL", {});
    arbusto.newVecBranch<float>("Muon_mvaTTHUL", {});
    
  }

  virtual void initCutflow()
  {
    // Initialize variables needed in cutflow.
    // Note: They will be set in the event loop by the functions called by the cuts
    cutflow.globals.newVar<LorentzVectors>("veto_lep_p4s", {});
    cutflow.globals.newVar<Integers>("veto_lep_pdgIDs", {});
    cutflow.globals.newVar<Integers>("veto_lep_idxs", {});
    cutflow.globals.newVar<Integers>("veto_lep_jet_idxs", {});
    cutflow.globals.newVar<LorentzVectors>("tight_lep_p4s", {});
    cutflow.globals.newVar<Integers>("tight_lep_pdgIDs", {});
    cutflow.globals.newVar<LorentzVectors>("jet_p4s", {});
    cutflow.globals.newVar<double>("ht_ak8", -999);

    // All events
    Cut *cut_base = new LambdaCut("AllEvents", [&]()
                                  { return true; });
    cutflow.setRoot(cut_base);

    // Pass Event filters --> move if they change per channel
    Cut *cut_passEventFilters = new LambdaCut("PassEventFilters", [&]()
                                              { return passEventFilters(); });
    cutflow.insert(cut_base, cut_passEventFilters, Right);

    // Cut *find_leps = new FindLeptons("FindLeptonsTTHUL", arbusto, nt, cli, cutflow);
    // cutflow.insert(cut_passEventFilters, find_leps, Right);

    // Lepton selection
    Cut *cut_noVetoLeps = new LambdaCut(
        "NoVetoLeptons",
        [&]()
        {
          return (cutflow.globals.getVal<LorentzVectors>("veto_lep_p4s").size() == 0);
        });
    cutflow.insert(cut_passEventFilters, cut_noVetoLeps, Right);

    Cut *cut_last = new LambdaCut("TheEnd", [&]()
                                  { return true; });
    cutflow.insert(cut_noVetoLeps, cut_last, Right);
  }

  virtual void initPerTTree()
  {
    // Golden JSON
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

  }

  virtual void runPerEvent(){

      if (cli.is_signal && cli.dump_truth)
      {
        truthAna.setTruthCandidates(); 
      }

      leptonSelection.selectVetoLeptons();

      // Run cutflow
      std::vector<std::string>
              cuts_to_check = {
          "TheEnd"};
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

};

#endif