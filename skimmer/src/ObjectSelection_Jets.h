#ifndef OBJECTSELECTION_JETS_H
#define OBJECTSELECTION_JETS_H

// NanoTools
#include "Base.h"
#include "Config.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
#include "Nano.h"
#include "Config.h"             // gconf

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

    void selectJets()
    {
      selectAK8Jets();
      selectAK8JetsRun2();
      selectAK4Jets();
      findNvbsJetPairs();
    }

    void selectAK4Jets()
    {
      LorentzVectors jet_p4s = {};
      double ht_ak4 = 0.;
      for (unsigned int jet_i = 0; jet_i < nt.nJet(); jet_i++)
      {
        LorentzVector jet_p4 = nt.Jet_p4().at(jet_i);
        bool pass_jet_id = ((nt.year() == 2016 && nt.Jet_jetId().at(jet_i) >= 1) || (nt.year() > 2016 && nt.Jet_jetId().at(jet_i) >= 2));
        if (jet_p4.pt() > 20 && pass_jet_id)
        {
          jet_p4s.push_back(jet_p4);
          ht_ak4 += jet_p4.pt();
        }
      }
      globals.setVal<LorentzVectors>("ak4jets_p4s", jet_p4s);
      globals.setVal<double>("ht_ak4", ht_ak4);
      globals.setVal<int>("n_ak4jets", jet_p4s.size());
    }

    void findNvbsJetPairs()
    {
      LorentzVectors jet_p4s = globals.getVal<LorentzVectors>("ak4jets_p4s");
      int n_vbsjet_pairs = 0;
      for (unsigned int jet_i = 0; jet_i < jet_p4s.size(); ++jet_i)
      {
        for (unsigned int jet_j = jet_i + 1; jet_j < jet_p4s.size(); ++jet_j)
        {
          LorentzVector jet1_p4 = jet_p4s.at(jet_i);
          LorentzVector jet2_p4 = jet_p4s.at(jet_j);
          double M_jj = (jet1_p4 + jet2_p4).M();
          double abs_deta_jj = fabs(jet1_p4.eta() - jet2_p4.eta());
          if (M_jj > 250 && abs_deta_jj > 2.5)
          {
            n_vbsjet_pairs++;
          }
        }
      }
      globals.setVal<int>("n_vbsjet_pairs", n_vbsjet_pairs);
    }

    // Run 2 selection 
    void selectAK8JetsRun2()
    {
      LorentzVectors fatjet_p4s = {};
      double ht_ak8 = 0.;
      
      for (unsigned int fatjet_i = 0; fatjet_i < nt.nFatJet(); fatjet_i++)
      {
        LorentzVector fatjet_p4 = nt.FatJet_p4().at(fatjet_i);
        if (fatjet_p4.pt() > 300 && fabs(fatjet_p4.eta()) < 2.5 && fatjet_p4.mass() > 50 && nt.FatJet_msoftdrop().at(fatjet_i) > 40 && nt.FatJet_jetId().at(fatjet_i) > 0)
        {
          fatjet_p4s.push_back(fatjet_p4);
          ht_ak8 += fatjet_p4.pt();
        }
      }

      globals.setVal<LorentzVectors>("ak8jets_run2sel_p4s", fatjet_p4s);
      globals.setVal<double>("ht_ak8_run2sel", ht_ak8);
      globals.setVal<int>("n_ak8jets_run2sel", fatjet_p4s.size());
    }

    // Looser selection 
    void selectAK8Jets()
    {
      LorentzVectors fatjet_p4s = {};
      double ht_ak8 = 0.;

      for (unsigned int fatjet_i = 0; fatjet_i < nt.nFatJet(); fatjet_i++)
      {
        LorentzVector fatjet_p4 = nt.FatJet_p4().at(fatjet_i);
        if (fatjet_p4.pt() > 250 && 
            fabs(fatjet_p4.eta()) < 2.5 && 
            nt.FatJet_msoftdrop().at(fatjet_i) > 40 && 
            nt.FatJet_jetId().at(fatjet_i) > 0)
        {
          fatjet_p4s.push_back(fatjet_p4);
          ht_ak8 += fatjet_p4.pt();
        }
      }

      globals.setVal<LorentzVectors>("ak8jets_p4s", fatjet_p4s);
      globals.setVal<double>("ht_ak8", ht_ak8);
      globals.setVal<int>("n_ak8jets", fatjet_p4s.size());
    }

};
#endif
