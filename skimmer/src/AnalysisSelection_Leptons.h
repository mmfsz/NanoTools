#ifndef ANALYSISSELECTION_LEPTONS_H
#define ANALYSISSELECTION_LEPTONS_H

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
#include "mvaTTH.h"

#include "ObjectSelection_Base.h"
#include "utilities.h" // Utilities::Variables


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
  //   Arbusto &arbusto;
  //   Nano &nt;
  //   HEPCLI &cli;
  //   Utilities::Variables &globals;

    LeptonSelection(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Utilities::Variables &cutflow_globals_ref)
        : ObjectSelection(arbusto_ref, nt_ref, cli_ref, cutflow_globals_ref)
        {
           // Do nothing
        };

    void computeLeptonMVA_ttHUL()
    {
      MVATTH::MVATTH mvatth2016("./data/leptonMVA/UL20_2016.xml");
      MVATTH::MVATTH mvatth2016APV("./data/leptonMVA/UL20_2016APV.xml");
      MVATTH::MVATTH mvatth2017("./data/leptonMVA/UL20_2017.xml");
      MVATTH::MVATTH mvatth2018("./data/leptonMVA/UL20_2018.xml");
      for (unsigned int elec_i = 0; elec_i < nt.nElectron(); ++elec_i)
      {
        float tth_mva = -99; 
        if (gconf.year == 2016 && gconf.isAPV) { tth_mva = mvatth2016APV.computeElecMVA(elec_i); }
        else if (gconf.year == 2016) { tth_mva = mvatth2016.computeElecMVA(elec_i); }
        else if (gconf.year == 2017) { tth_mva = mvatth2017.computeElecMVA(elec_i); }
        else if (gconf.year == 2018) { tth_mva = mvatth2018.computeElecMVA(elec_i); }
        else { std::cout << "LeptonSelection::computeLeptonMVA_ttHUL(): Something is wrong with the year." << std::endl; abort();}
        arbusto.appendToVecLeaf<float>("Electron_mvaTTHUL", tth_mva);
        electronMVA_.push_back(tth_mva);
      }
      for (unsigned int muon_i = 0; muon_i < nt.nMuon(); ++muon_i)
      {
        float tth_mva = nt.Muon_mvaTTH().at(muon_i);
        arbusto.appendToVecLeaf<float>("Muon_mvaTTHUL", tth_mva);
      }
    }

    bool passVetoElecID(unsigned int elec_i)
    {
      return ttH_UL::electronID(elec_i, ttH::IDveto, nt.year(), electronMVA_);
    }

    bool passVetoMuonID(unsigned int muon_i)
    {
      return ttH_UL::muonID(muon_i, ttH::IDveto, nt.year());
    }

    virtual bool passTightElecID(int elec_i)
    {
      return ttH_UL::electronID(elec_i, ttH::IDtight, nt.year(), electronMVA_);
    }

    virtual bool passTightMuonID(int muon_i)
    {
      return ttH_UL::muonID(muon_i, ttH::IDtight, nt.year());
    }

    void selectVetoLeptons()
    {
      // Commeting out since not needed for all-had and increases executiong time by x20
      //computeLeptonMVA_ttHUL();

      LorentzVectors veto_lep_p4s;
      Integers veto_lep_pdgIDs;
      Integers veto_lep_idxs;
      Integers veto_lep_jet_idxs;
      //LorentzVectors tight_lep_p4s;
      //Integers tight_lep_pdgIDs;
      
      for (unsigned int elec_i = 0; elec_i < nt.nElectron(); elec_i++)
      {
        LorentzVector lep_p4 = nt.Electron_p4().at(elec_i);
        if (passVetoElecID(elec_i))
        {
          veto_lep_p4s.push_back(lep_p4);
          veto_lep_pdgIDs.push_back(-nt.Electron_charge().at(elec_i) * 11);
        }
        // if (passTightElecID(elec_i))
        // {
        //   tight_lep_p4s.push_back(lep_p4);
        //   tight_lep_pdgIDs.push_back(-nt.Electron_charge().at(elec_i) * 11);
        // }
      }
      for (unsigned int muon_i = 0; muon_i < nt.nMuon(); muon_i++)
      {
        LorentzVector lep_p4 = nt.Muon_p4().at(muon_i);
        if (passVetoMuonID(muon_i))
        {
          veto_lep_p4s.push_back(lep_p4);
          veto_lep_pdgIDs.push_back(-nt.Muon_charge().at(muon_i) * 13);
        }
        // if (passTightMuonID(muon_i))
        // {
        //   tight_lep_p4s.push_back(lep_p4);
        //   tight_lep_pdgIDs.push_back(-nt.Muon_charge().at(muon_i) * 13);
        // }
      }

      globals.setVal<LorentzVectors>("veto_lep_p4s", veto_lep_p4s);
      globals.setVal<Integers>("veto_lep_pdgIDs", veto_lep_pdgIDs);
      globals.setVal<Integers>("veto_lep_idxs", veto_lep_idxs);
      globals.setVal<Integers>("veto_lep_jet_idxs", veto_lep_jet_idxs);

      //globals.setVal<LorentzVectors>("tight_lep_p4s", tight_lep_p4s);
      //globals.setVal<Integers>("tight_lep_pdgIDs", tight_lep_pdgIDs);

      electronMVA_.clear();

    }
};
#endif
