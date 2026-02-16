#ifndef OBJECTSELECTION_JETS_H
#define OBJECTSELECTION_JETS_H

// NanoTools
#include "Base.h"
#include "Config.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "Nano.h"
#include "Config.h"             // gconf

#include <cmath>
#include "arbol.h"
#include "arbusto.h"
#include "cutflow.h"
#include "hepcli.h"
// Custom skimmer
#include "ObjectSelection_Base.h"
#include "utilities.h" // Utilities::Variables
// ROOT
#include "TString.h"

typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class JetSelection : public ObjectSelection
{
  public:


    JetSelection(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Utilities::Variables &cutflow_globals_ref)
        : ObjectSelection(arbusto_ref, nt_ref, cli_ref, cutflow_globals_ref)
        {
           // Do nothing
        };

    // DeltaR calculation
    float deltaR(const LorentzVector &v1, const LorentzVector &v2)
    {
      float dEta = v1.eta() - v2.eta();
      float dPhi = v1.phi() - v2.phi();
      if (dPhi > M_PI) dPhi -= 2*M_PI;
      if (dPhi < -M_PI) dPhi += 2*M_PI;
      return std::sqrt(dEta*dEta + dPhi*dPhi);
    }

    // Veto Jets that overlap with a veto lepton
    bool passLeptonDRCut(const LorentzVector &jet, const LorentzVectors &leptons, double minDr = 0.4)
    {
      for (const auto &lep : leptons)
      {
        if (deltaR(jet, lep) <= minDr)
          return false;
      }
      return true;
    }

    void selectVVHJets()
    {
      selectVVHAK4Jets();
      selectVVHAK8Jets();
    }

    void selectVVHAK4Jets()
    {
      LorentzVectors jet_p4s = {};
      LorentzVectors vvh_veto_lep_p4s = globals.getVal<LorentzVectors>("vvh_veto_lep_p4s");
      for (unsigned int jet_i = 0; jet_i < nt.nJet(); jet_i++)
      {
        LorentzVector jet_p4 = nt.Jet_p4().at(jet_i);
        if (jet_p4.pt() > 15) //&& passLeptonDRCut(jet_p4, vvh_veto_lep_p4s))
        {
          jet_p4s.push_back(jet_p4);
        }
      }
      globals.setVal<LorentzVectors>("vvh_veto_jet_p4s", jet_p4s);
      globals.setVal<int>("n_vvh_veto_jets", jet_p4s.size());
    }

    void selectVVHAK8Jets()
    {
      LorentzVectors fatjet_p4s = {};
      for (unsigned int fatjet_i = 0; fatjet_i < nt.nFatJet(); fatjet_i++)
      {
        LorentzVector fatjet_p4 = nt.FatJet_p4().at(fatjet_i);
        if (fatjet_p4.pt() > 200 && nt.FatJet_msoftdrop().at(fatjet_i) > 20) 
        {
          fatjet_p4s.push_back(fatjet_p4);
        }
      }
      globals.setVal<LorentzVectors>("vvh_veto_fatjet_p4s", fatjet_p4s);
      globals.setVal<int>("n_vvh_veto_fatjets", fatjet_p4s.size());
    }

};
#endif
