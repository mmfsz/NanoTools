#ifndef Analysis_LepVeto_2AK8_H
#define Analysis_LepVeto_2AK8_H

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

#include "Analysis.h"
#include "ObjectSelection_Leptons.h"
#include "ObjectSelection_Jets.h"

typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class Analysis_LepVeto_2AK8 : public Analysis
{
public:

  // Constructor
  Analysis_LepVeto_2AK8(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
      : Analysis(arbusto_ref, nt_ref, cli_ref, cutflow_ref)
  {
  }

  // Initialize branches to be added to output "Events" TTree
  void initBranches() override
  {
    Analysis::initBranches();

    // These actually create empty branches if the final skimmer cut is after NoVetoLeptons
    // arbusto.newVecBranch<int>("veto_lep_p4s", {});
    // arbusto.newVecBranch<unsigned int>("veto_lep_idxs", {});
    // arbusto.newVecBranch<int>("veto_lep_jet_idxs", {});
    // arbusto.newVecBranch<int>("veto_lep_pdgIDs", {});

  }

  // Define global variables and cutflow to be run in event loop
  // Note: the variables are set in the event loop when performing the object selection. 
  //       The cutflow has to be run after the appropriate object selection has been performed.

  void initCutflow() override
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


    // Pass Event filters 
    Cut *cut_passEventFilters = new LambdaCut("PassEventFilters", [&]()
                                              { return passEventFilters(); });
    vCutflowCuts_.push_back(cut_passEventFilters);


    // Lepton selection
    Cut *cut_noVetoLeps = new LambdaCut(
        "NoVetoLeptons",
        [&]()
        {
          return (cutflow.globals.getVal<LorentzVectors>("veto_lep_p4s").size() == 0);
        });
    vCutflowCuts_.push_back(cut_noVetoLeps);

    // Jet selection
    Cut *cut_AtLeast2AK8Jets = new LambdaCut(
        "AtLeast2AK8Jets",
        [&]()
        {
          return (cutflow.globals.getVal<int>("n_ak8jets") >= 2);
        });
    vCutflowCuts_.push_back(cut_AtLeast2AK8Jets);

    Analysis::initCutflow();
  }

};

#endif