
#include "vvhjj_allhad.h"
#include "ElectronSelections.h"
#include "MuonSelections.h"
bool DEBUG = false;

// bool runSelection_allHad(Nano &nt) //, Arbusto &arbusto)
// {
//     // Lepton selection 
//     //computeLeptonMVA_ttHUL(nt, arbusto);

//     bool passLeptonSelection = runLeptonSelection();
//     if (!passLeptonSelection) return false;

//     // Jets selection: 3 hadronic decays + two forward jets 
//     //if (!runJetsSelection()) return false;

//     if (!runJetsSelection_Run2()) return false;

//     // Trigger selection? 

//     return true;
// }

//////////////////////////////////////////////////////////
//                      Events filters                    //
//////////////////////////////////////////////////////////
bool passEventFilters(Nano &nt)
{
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

      // "( (is2016) || Flag_ecalBadCalibFilter) &&" // apply only to 2017, 2018
      // "( (!isData) || Flag_globalSuperTightHalo2016Filter)", // apply only to data
      // "EventFilters");

//////////////////////////////////////////////////////////
//                      Leptons                         //
//////////////////////////////////////////////////////////
// Cannot include arbust.h again??
// void computeLeptonMVA_ttHUL(Nano nt, Arbusto arbusto){
//     MVATTH::MVATTH mvatth("./data/leptonMVA/UL20_2018.xml");

//     for (unsigned int elec_i = 0; elec_i < nt.nElectron(); ++elec_i)
//     {
//         float tth_mva = mvatth.computeElecMVA(elec_i);
//         arbusto.appendToVecLeaf<float>("Electron_mvaTTHUL", tth_mva);
//     }
//     for (unsigned int muon_i = 0; muon_i < nt.nMuon(); ++muon_i)
//     {
//         float tth_mva = nt.Muon_mvaTTH().at(muon_i);
//         arbusto.appendToVecLeaf<float>("Muon_mvaTTHUL", tth_mva);
//     }

// }

/* NOTE: The ttH_UL functions are added only in this development branch.
 *       They use ttH UL MVA branches, which so do not exist in v9 NanoAOD production and are manually added as custom branches.
 */
bool passVetoElecID(unsigned int elec_i, Nano& nt) {
    //return ttH::electronID(elec_i, ttH::IDveto, nt.year());
    return ttH_UL::electronID(elec_i, ttH::IDveto, nt.year());
}

bool passVetoMuonID(unsigned int muon_i, Nano& nt) {
    //return ttH::muonID(muon_i, ttH::IDveto, nt.year());
    return ttH_UL::muonID(muon_i, ttH::IDveto, nt.year());
}

bool passLeptonVeto(Nano &nt)
{
    // Select leptons candidates with losest ttH selection
    LorentzVectors veto_lep_p4s;
    Integers veto_lep_pdgIDs; // TO DELETE if not used
    Integers veto_lep_idxs; // 
    Integers veto_lep_jet_idxs; //
    // Loop over electrons
    for (unsigned int i = 0; i < nt.nElectron(); ++i)
    {
        if (!passVetoElecID(i, nt))
        {
            continue;
        }
        LorentzVector el_p4 = nt.Electron_p4().at(i);
        // Store basic info
        veto_lep_p4s.push_back(el_p4);
        veto_lep_pdgIDs.push_back(-nt.Electron_charge().at(i) * 11);
        veto_lep_idxs.push_back(i);
        veto_lep_jet_idxs.push_back(nt.Electron_jetIdx().at(i));
    }
    // Loop over muons
    for (unsigned int i = 0; i < nt.nMuon(); ++i)
    {
        if (!passVetoMuonID(i, nt))
        {
            continue;
        }
        LorentzVector mu_p4 = nt.Muon_p4().at(i);
        // Store basic info
        veto_lep_p4s.push_back(mu_p4);
        veto_lep_pdgIDs.push_back(-nt.Muon_charge().at(i) * 13);
        veto_lep_idxs.push_back(i);
        veto_lep_jet_idxs.push_back(nt.Muon_jetIdx().at(i));
    }
    // Add new variables to output tree TO DO !!
    // globals.setVal<LorentzVectors>("veto_lep_p4s", veto_lep_p4s);
    // globals.setVal<Integers>("veto_lep_pdgIDs", veto_lep_pdgIDs);
    // globals.setVal<Integers>("veto_lep_idxs", veto_lep_idxs);
    // globals.setVal<Integers>("veto_lep_jet_idxs", veto_lep_jet_idxs);
    return (veto_lep_p4s.size() == 0);
}

//////////////////////////////////////////////////////////
//                         Jets                         //
//////////////////////////////////////////////////////////

bool runJetsSelection(Nano &nt)
{
    LorentzVectors selectedAK4jets = selectAK4Jets(nt);

    // Require at least two VBS jets
    if (!runVBSSelection(selectedAK4jets)) return false;

    LorentzVectors selectedAK8jets = selectAK8Jets(nt);
    double ht_vbs{0.};
    for (auto &vbsjet : selectedAK8jets) {
      ht_vbs += vbsjet.pt();
    }

    // Require 3 hadronic decays plus 2 VBS jets
    auto n_AK8jets = selectedAK8jets.size();
    auto n_AK4jets = selectedAK4jets.size();    
    if ((2*n_AK8jets+n_AK4jets) < 8) return false;

    // Require hadronic activity (remove vbs jets contribution)
    double ht_ak8 {0.};
    double ht_ak4 {0.};
    for (auto &ak8jet : selectedAK8jets) {
      ht_ak8 += ak8jet.pt();
    }
    for (auto &ak4jet : selectedAK4jets) {
      ht_ak4 += ak4jet.pt();
    }

    if ( (ht_ak8+ht_ak4-ht_vbs) < 250) return false;

    return true;    
}

// Select for only boosted scenario
bool runJetsSelection_Run2(Nano &nt)
{
    if (DEBUG) std::cout << "runJetsSelection_Run2(): Entered. " << std::endl;

    LorentzVectors selectedAK8jets = selectAK8Jets(nt);

    auto n_AK8jets = selectedAK8jets.size();

    // Require at least 2 fat jets
    if (n_AK8jets < 2) return false;

    // Require hadronic activity from fat jets
    double ht_ak8 {0.};
    for (auto& ak8jet: selectedAK8jets)
    {
        ht_ak8 += ak8jet.pt();
    }
    if (ht_ak8 <= 1100) return false;

    if (DEBUG) std::cout << "runJetsSelection_Run2(): Exiting." << std::endl;
    return true;    
}


LorentzVectors selectAK4Jets(Nano &nt){
    LorentzVectors jet_p4s = {};
    for (unsigned int jet_i = 0; jet_i < nt.nJet(); jet_i++)
    {
        LorentzVector jet_p4 = nt.Jet_p4().at(jet_i);
        bool pass_jet_id = (
            (nt.year() == 2016 && nt.Jet_jetId().at(jet_i) >= 1)
            || (nt.year() > 2016 && nt.Jet_jetId().at(jet_i) >= 2)
        );
        if (jet_p4.pt() > 20 && pass_jet_id)
        {
            jet_p4s.push_back(jet_p4);
        }
    }
    return jet_p4s;
}

bool runVBSSelection(LorentzVectors jet_p4s){

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
    return (n_vbsjet_pairs > 0);
}

LorentzVectors selectAK8Jets(Nano &nt)
{
    LorentzVectors fatjet_p4s = {};
    for (unsigned int fatjet_i = 0; fatjet_i < nt.nFatJet(); fatjet_i++)
    {
        LorentzVector fatjet_p4 = nt.FatJet_p4().at(fatjet_i);
        if (
            fatjet_p4.pt() > 300 
            && fabs(fatjet_p4.eta()) < 2.5 
            && fatjet_p4.mass() > 50 
            && nt.FatJet_msoftdrop().at(fatjet_i) > 40 
            && nt.FatJet_jetId().at(fatjet_i) > 0
            )
        {
            fatjet_p4s.push_back(fatjet_p4);
        }
    }
    return fatjet_p4s;
}