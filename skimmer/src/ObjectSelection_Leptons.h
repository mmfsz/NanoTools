#ifndef OBJECTSELECTION_LEPTONS_H
#define OBJECTSELECTION_LEPTONS_H

// NanoTools
#include "Base.h"
#include "Config.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "Nano.h"
#include "Config.h"

#include "arbol.h"
#include "arbusto.h"
#include "cutflow.h"
#include "hepcli.h"

// Custom skimmer
#include "mvaTTH.h"
#include "ObjectSelection_Base.h"
#include "utilities.h"

// ROOT
#include "TString.h"

typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class LeptonSelection : public ObjectSelection
{
  public:
    std::vector<float> electronMVA_;

    LeptonSelection(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Utilities::Variables &cutflow_globals_ref)
        : ObjectSelection(arbusto_ref, nt_ref, cli_ref, cutflow_globals_ref)
        {
           // Do nothing
        };

    bool passVVHVetoElecID(unsigned int elec_i)
    {
      return VVH::electronID(elec_i, VVH::IDveto, nt.year());
    }

    bool passVVHVetoMuonID(unsigned int muon_i)
    {
      return VVH::muonID(muon_i, VVH::IDveto, nt.year());
    }

    void selectVVHVetoLeptons()
    {
      LorentzVectors vvh_veto_lep_p4s;
      double vvh_lep_pt_lead = 0.;
      double vvh_lep_pt_sub = 0.;

      for (unsigned int elec_i = 0; elec_i < nt.nElectron(); elec_i++)
      {
        LorentzVector lep_p4 = nt.Electron_p4().at(elec_i);
        if (passVVHVetoElecID(elec_i))
        {
          vvh_veto_lep_p4s.push_back(lep_p4);
        }
      }

      for (unsigned int muon_i = 0; muon_i < nt.nMuon(); muon_i++)
      {
        LorentzVector lep_p4 = nt.Muon_p4().at(muon_i);
        if (passVVHVetoMuonID(muon_i))
        {
          vvh_veto_lep_p4s.push_back(lep_p4);
        }
      }

      // Sort by pT
      std::sort(
        vvh_veto_lep_p4s.begin(),
        vvh_veto_lep_p4s.end(),
        [](const LorentzVector& a, const LorentzVector& b)
        {
          return a.pt() > b.pt();
        }
      );

      // Set leading and subleading pT
      if (!vvh_veto_lep_p4s.empty())
        vvh_lep_pt_lead = vvh_veto_lep_p4s.at(0).pt();
      if (vvh_veto_lep_p4s.size() > 1)
        vvh_lep_pt_sub = vvh_veto_lep_p4s.at(1).pt();

      globals.setVal<LorentzVectors>("vvh_veto_lep_p4s", vvh_veto_lep_p4s);
      globals.setVal<double>("vvh_lep_pt_lead", vvh_lep_pt_lead);
      globals.setVal<double>("vvh_lep_pt_sub", vvh_lep_pt_sub);
    }

};
#endif
