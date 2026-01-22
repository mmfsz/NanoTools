#ifndef CORE_CUTS_H
#define CORE_CUTS_H

// RAPIDO
#include "arbol.h"
#include "arbusto.h"
#include "cutflow.h" // Cut
#include "hepcli.h"
#include "utilities.h" // Utilities::Variables

// Nano
#include "ElectronSelections.h"
#include "MuonSelections.h"

// local
#include "mvaTTH.h"


typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class SkimmerCut : public Cut
{
public:
    Arbusto& arbusto;
    Nano& nt;
    HEPCLI& cli;
    Utilities::Variables& globals;

    // SkimmerCut(std::string new_name, Analysis& s) 
    // : Cut(new_name), arbusto(s.arbusto), nt(s.nt), cli(s.cli), globals(s.cutflow.globals)
    // {
    //     // Do nothing
    // };

    SkimmerCut(std::string cut_name, Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
      : Cut(cut_name),
        arbusto(arbusto_ref),
        nt(nt_ref), 
        cli(cli_ref),
        globals(cutflow_ref.globals)
        {
        };
};

class FindLeptons : public SkimmerCut
{
public:
    std::vector<float> electronMVA_;
    // FindLeptons(std::string name, Analysis &skimmer) : SkimmerCut(name, skimmer){
    //                                                             // Do nothing
    //                                                         };
    FindLeptons(std::string cut_name, Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
      : SkimmerCut(cut_name, arbusto_ref, nt_ref, cli_ref, cutflow_ref)
        {
        };

    /* NOTE: The branch Electron_mvaTTHUL from ttH UL analysis does not exists in v9 NanoAOD production.
    *       The mva_tthUL for electrons is manually computed at run time and added to the arbusto tree.
    *       Electron_mvaTTHUL is a vector filled per-event with the tth_mva scores of each electron in the nt.Electron collection.
    *       The muon mva_tth branche stored in nt is fine. 
    */
    void computeLeptonMVA_ttHUL()
    {
        MVATTH::MVATTH mvatth("./data/leptonMVA/UL20_2018.xml"); // FIXME
        
        for (unsigned int elec_i = 0; elec_i < nt.nElectron(); ++elec_i)
        {
            float tth_mva = mvatth.computeElecMVA(elec_i);
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

    bool passVVHVetoElecID(unsigned int elec_i)
    {
        return VVH::electronID(elec_i, VVH::IDveto, nt.year());
    }

    bool passVetoMuonID(unsigned int muon_i)
    {
        return ttH_UL::muonID(muon_i, ttH::IDveto, nt.year());
    }

    bool passVVHVetoMuonID(unsigned int muon_i)
    {
        return VVH::muonID(muon_i, VVH::IDveto, nt.year());
    }

    virtual bool passTightElecID(int elec_i)
    {
        return ttH_UL::electronID(elec_i, ttH::IDtight, nt.year(), electronMVA_);
    }

    virtual bool passTightMuonID(int muon_i)
    {
        return ttH_UL::muonID(muon_i, ttH::IDtight, nt.year());
    }

    bool evaluate()
    {
        computeLeptonMVA_ttHUL();

        LorentzVectors veto_lep_p4s;
        LorentzVectors vvh_veto_lep_p4s;
        LorentzVectors tight_lep_p4s;
        Integers veto_lep_pdgIDs;
        Integers tight_lep_pdgIDs;
        for (unsigned int elec_i = 0; elec_i < nt.nElectron(); elec_i++)
        {
            LorentzVector lep_p4 = nt.Electron_p4().at(elec_i);
            if (passVetoElecID(elec_i))
            {
                veto_lep_p4s.push_back(lep_p4);
                veto_lep_pdgIDs.push_back(-nt.Electron_charge().at(elec_i) * 11);
            }
            if (passVVHVetoElecID(elec_i))
            {
                vvh_veto_lep_p4s.push_back(lep_p4);
            }
            if (passTightElecID(elec_i))
            {
                tight_lep_p4s.push_back(lep_p4);
                tight_lep_pdgIDs.push_back(-nt.Electron_charge().at(elec_i) * 11);
            }
        }
        for (unsigned int muon_i = 0; muon_i < nt.nMuon(); muon_i++)
        {
            LorentzVector lep_p4 = nt.Muon_p4().at(muon_i);
            if (passVetoMuonID(muon_i))
            {
                veto_lep_p4s.push_back(lep_p4);
                veto_lep_pdgIDs.push_back(-nt.Muon_charge().at(muon_i) * 13);
            }
            if (passVVHVetoMuonID(muon_i))
            {
                vvh_veto_lep_p4s.push_back(lep_p4);
            }
            if (passTightMuonID(muon_i))
            {
                tight_lep_p4s.push_back(lep_p4);
                tight_lep_pdgIDs.push_back(-nt.Muon_charge().at(muon_i) * 13);
            }
        }

        globals.setVal<LorentzVectors>("veto_lep_p4s", veto_lep_p4s);
        globals.setVal<LorentzVectors>("vvh_veto_lep_p4s", vvh_veto_lep_p4s);
        globals.setVal<LorentzVectors>("tight_lep_p4s", tight_lep_p4s);
        globals.setVal<Integers>("veto_lep_pdgIDs", veto_lep_pdgIDs);
        globals.setVal<Integers>("tight_lep_pdgIDs", tight_lep_pdgIDs);

        electronMVA_.clear();

        return true;
    };
};

#endif
