#ifndef Analysis_LepVeto_H
#define Analysis_LepVeto_H

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

class Analysis_LepVeto : public Analysis
{
public:

  // Constructor
  Analysis_LepVeto(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
      : Analysis(arbusto_ref, nt_ref, cli_ref, cutflow_ref)
  {
  }

  // Initialize branches to be added to output "Events" TTree
  void initBranches() override
  {
    Analysis::initBranches();

  }

  // Define global variables and cutflow to be run in event loop
  // Note: the variables are set in the event loop when performing the object selection. 
  //       The cutflow has to be run after the appropriate object selection has been performed.

  void initCutflow() override
  {
    // Lepton selection
    Cut *cut_noVetoLeps = new LambdaCut(
        "NoVetoLeptons",
        [&]()
        {
          return (cutflow.globals.getVal<LorentzVectors>("veto_lep_p4s").size() == 0);
        });
    vCutflowCuts_.push_back(cut_noVetoLeps);

    Analysis::initCutflow();
  }

};

#endif