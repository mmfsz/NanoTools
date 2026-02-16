#ifndef TRUTH_ANALYSIS_H
#define TRUTH_ANALYSIS_H

// RAPIDO
#include "arbol.h"
#include "arbusto.h"
#include "cutflow.h"
#include "hepcli.h"
// ROOT
#include "TString.h"
// NanoCORE
#include "Nano.h"



struct TruthAnalysis
{
  Arbusto &arbusto;
  Nano &nt;
  HEPCLI &cli;
  Cutflow &cutflow;

  TruthAnalysis(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Cutflow &cutflow_ref)
      : arbusto(arbusto_ref), nt(nt_ref), cli(cli_ref), cutflow(cutflow_ref){
        std::cout << "Initialize Truth Analysis object" << std::endl;
      };

  void initTruthBranches(){
    if (cli.is_signal && cli.dump_truth)
    {
      arbusto.newBranch<float>("truthH_pt", -99.);
      arbusto.newBranch<float>("truthH_eta", -99.);
      arbusto.newBranch<float>("truthH_phi", -99.);
      arbusto.newBranch<float>("truthH_mass", -99.);
      arbusto.newBranch<int>("truthH_pdgId", 0);
      arbusto.newBranch<int>("truthH_genPartIdxMother", -99);
      arbusto.newBranch<int>("truthH_status", -99);
      // store gen indices to match branch names in run 3 framework skimmer
      arbusto.newBranch<int>("gen_h_idx", -1);
      arbusto.newBranch<int>("gen_b1_idx", -1);
      arbusto.newBranch<int>("gen_b2_idx", -1);
      arbusto.newBranch<int>("gen_v1_idx", -1);
      arbusto.newBranch<int>("gen_v1q1_idx", -1);
      arbusto.newBranch<int>("gen_v1q2_idx", -1);
      arbusto.newBranch<int>("gen_v2_idx", -1);
      arbusto.newBranch<int>("gen_v2q1_idx", -1);
      arbusto.newBranch<int>("gen_v2q2_idx", -1);
      arbusto.newBranch<int>("gen_vbs1_idx", -1);
      arbusto.newBranch<int>("gen_vbs2_idx", -1);

      for (size_t i = 1; i < 3; ++i)
      {
        std::string idx = std::to_string(i);
        arbusto.newBranch<float>("truthH_daught" + idx + "_pt", -99.);
        arbusto.newBranch<float>("truthH_daught" + idx + "_eta", -99.);
        arbusto.newBranch<float>("truthH_daught" + idx + "_phi", -99.);
        arbusto.newBranch<float>("truthH_daught" + idx + "_mass", -99.);
        arbusto.newBranch<int>("truthH_daught" + idx + "_pdgId", 0);
        arbusto.newBranch<int>("truthH_daught" + idx + "_genPartIdxMother", -99);
        arbusto.newBranch<int>("truthH_daught" + idx + "_status", -99);

        arbusto.newBranch<float>("truthVBSq" + idx + "_pt", -99.);
        arbusto.newBranch<float>("truthVBSq" + idx + "_eta", -99.);
        arbusto.newBranch<float>("truthVBSq" + idx + "_phi", -99.);
        arbusto.newBranch<float>("truthVBSq" + idx + "_mass", -99.);
        arbusto.newBranch<int>("truthVBSq" + idx + "_pdgId", 0);
        arbusto.newBranch<int>("truthVBSq" + idx + "_genPartIdxMother", -99.);
        arbusto.newBranch<int>("truthVBSq" + idx + "_status", -99);

        arbusto.newBranch<float>("truthV" + idx + "_pt", -99.);
        arbusto.newBranch<float>("truthV" + idx + "_eta", -99.);
        arbusto.newBranch<float>("truthV" + idx + "_phi", -99.);
        arbusto.newBranch<float>("truthV" + idx + "_mass", -99.);
        arbusto.newBranch<int>("truthV" + idx + "_pdgId", 0);
        arbusto.newBranch<int>("truthV" + idx + "_genPartIdxMother", -99);
        arbusto.newBranch<int>("truthV" + idx + "_status", -99);
        for (size_t j = 1; j < 3; ++j)
        {
          std::string idx2 = std::to_string(j);
          arbusto.newBranch<float>("truthV" + idx + "_daught" + idx2 + "_pt", -99.);
          arbusto.newBranch<float>("truthV" + idx + "_daught" + idx2 + "_eta", -99.);
          arbusto.newBranch<float>("truthV" + idx + "_daught" + idx2 + "_phi", -99.);
          arbusto.newBranch<float>("truthV" + idx + "_daught" + idx2 + "_mass", -99.);
          arbusto.newBranch<int>("truthV" + idx + "_daught" + idx2 + "_pdgId", 0);
          arbusto.newBranch<int>("truthV" + idx + "_daught" + idx2 + "_genPartIdxMother", -99);
          arbusto.newBranch<int>("truthV" + idx + "_daught" + idx2 + "_status", -99);
        }
      }
    }
  };

void setTruthCandidates() //Nano &nt, Arbusto &arbusto, HEPCLI &cli)
{
  int higgs_idx{-99};
  std::vector<int> hdecay_idx;
  std::vector<int> firstVs_idx;
  std::vector<float> firstVs_pt;
  std::vector<int> vbsquarks_idx;
  int nFoundHiggs{0};
  int nFoundVs{0};

  if (cli.debug)
  {
    std::cout << "============== EVENT STARTS HERE =============" << std::endl;
    dumpTruthEventInfo(nt);
  }

  // Find intermediate H, V1, V2 particles and H daughters
  for (size_t igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
  {
    int status = nt.GenPart_status().at(igen);
    int pdgId = nt.GenPart_pdgId().at(igen);
    int mother_idx = nt.GenPart_genPartIdxMother().at(igen);

    if (mother_idx == 0)
    {
      // One intermediate Higgs
      if ((status == 22) && (pdgId == 25))
      {
        if (cli.debug)
        {
          std::cout << "Found intermediate state Higgs" << std::endl;
          printTruthInfo(nt, igen);
        }
        higgs_idx = igen;
        nFoundHiggs++;
      }
      // Two intermediate W or Z
      else if ((status == 22) && (pdgId == 23 || abs(pdgId) == 24))
      {
        if (cli.debug)
        {
          std::cout << "Found intermediate state V" << std::endl;
          printTruthInfo(nt, igen);
        }
        nFoundVs++;
        firstVs_idx.push_back(igen);
        firstVs_pt.push_back(nt.GenPart_pt().at(igen));
      }
      // Two outgoing quarks with mother_idx == 0
      else if ((status == 23) && (abs(pdgId) >= 1 && abs(pdgId) <= 6))
      {

        vbsquarks_idx.push_back(igen);
      }
    }
  }

  // Check nothing unexpected happened
  if (nFoundHiggs != 1)
  {
    std::cerr << "Warning: could not find exactly one Higgs. Truth variables will not be set for this event." << std::endl;
    // dumpTruthEventInfo(nt);
    return;
  }
  else if (nFoundVs != 2)
  {
    // some non-vbs events have 3 V bosons
    std::cerr << "Warning: could not find exactly two V's. Truth variables will not be set for this event." << std::endl;
    // dumpTruthEventInfo(nt);
    return;
  }
  else if (vbsquarks_idx.size() != 2)
  {
    std::cerr << "Warning: could not find exactly 2 VBS quarks. Truth variables will not be set for this event." << std::endl;
    // dumpTruthEventInfo(nt);
    return;
  }

  // If you get here, truth variables will be set. Some daughters might be missing.

  // ==========================================

  // Set Higgs variables
  arbusto.setLeaf<float>("truthH_pt", nt.GenPart_pt().at(higgs_idx));
  arbusto.setLeaf<float>("truthH_eta", nt.GenPart_eta().at(higgs_idx));
  arbusto.setLeaf<float>("truthH_phi", nt.GenPart_phi().at(higgs_idx));
  arbusto.setLeaf<float>("truthH_mass", nt.GenPart_mass().at(higgs_idx));
  arbusto.setLeaf<int>("truthH_pdgId", nt.GenPart_pdgId().at(higgs_idx));
  arbusto.setLeaf<int>("truthH_genPartIdxMother", nt.GenPart_genPartIdxMother().at(higgs_idx));
  arbusto.setLeaf<int>("truthH_status", nt.GenPart_status().at(higgs_idx));
  arbusto.setLeaf<int>("gen_h_idx", higgs_idx);

  // Find last H boson in its decay chain
  int lastH_idx = findLastIndex(higgs_idx, nt.GenPart_pdgId().at(higgs_idx), nt);

  // Find daughters of last H boson
  for (size_t igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
  {
    int mother_idx = nt.GenPart_genPartIdxMother().at(igen);
    if (mother_idx == lastH_idx)
    {
      if (cli.debug)
      {
        std::cout << "Found a Higgs daughter" << std::endl;
        printTruthInfo(nt, igen);
      }
      hdecay_idx.push_back(igen);
    }
  }

  // Sort Higgs daughters by pt
  if (hdecay_idx.size() == 2)
  {
    std::sort(hdecay_idx.begin(), hdecay_idx.end(), [this](int a, int b) {
      return nt.GenPart_pt().at(a) > nt.GenPart_pt().at(b);
    });
  }
  else if (hdecay_idx.size() > 2)
  {
    std::cerr << "Error: more than 2 Higgs daughters were found. This should never happen!" << std::endl;
    dumpTruthEventInfo(nt);
    std::terminate();
  }
  else
  {
    std::cerr << "Warning: Less than 2 Higgs daughters were found." << std::endl;
  }

  for (size_t idaught = 0; idaught < hdecay_idx.size(); ++idaught)
  {
    int idaught_idx = hdecay_idx.at(idaught);
    arbusto.setLeaf<float>("truthH_daught" + std::to_string(idaught + 1) + "_pt", nt.GenPart_pt().at(idaught_idx));
    arbusto.setLeaf<float>("truthH_daught" + std::to_string(idaught + 1) + "_eta", nt.GenPart_eta().at(idaught_idx));
    arbusto.setLeaf<float>("truthH_daught" + std::to_string(idaught + 1) + "_phi", nt.GenPart_phi().at(idaught_idx));
    arbusto.setLeaf<float>("truthH_daught" + std::to_string(idaught + 1) + "_mass", nt.GenPart_mass().at(idaught_idx));
    arbusto.setLeaf<int>("truthH_daught" + std::to_string(idaught + 1) + "_pdgId", nt.GenPart_pdgId().at(idaught_idx));
    arbusto.setLeaf<int>("truthH_daught" + std::to_string(idaught + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(idaught_idx));
    arbusto.setLeaf<int>("truthH_daught" + std::to_string(idaught + 1) + "_status", nt.GenPart_status().at(idaught_idx));
    arbusto.setLeaf<int>("gen_b" + std::to_string(idaught + 1) + "_idx", idaught_idx);
  }

  // Set V bosons variables

  // Precompute info for each first V
  struct VInfo {
    int first_idx;
    int last_idx;
    std::vector<int> daughters;
    bool is_had;
    float first_pt;
  };

  // Set V bosons and their daughters
  std::vector<VInfo> v_infos;
  for (size_t iV = 0; iV < firstVs_idx.size(); ++iV)
  {
    int firstV_idx = firstVs_idx.at(iV);
    int firstV_pdgId = nt.GenPart_pdgId().at(firstV_idx);

    // Find last V boson in its decay chain
    int lastV_idx = findLastIndex(firstV_idx, firstV_pdgId, nt);

    // Find daughters of last V boson
    std::vector<int> vdecays_idx;
    for (size_t igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
    {
      int mother_idx = nt.GenPart_genPartIdxMother().at(igen);
      if (mother_idx == lastV_idx)
      {
        if (cli.debug)
        {
          std::cout << "Found a V daughter" << std::endl;
          printTruthInfo(nt, igen);
        }
        vdecays_idx.push_back(igen);
      }
    }

    // Sort V daughters by pt
    if (vdecays_idx.size() == 2)
    {
      std::sort(vdecays_idx.begin(), vdecays_idx.end(), [this](int a, int b) {
        return nt.GenPart_pt().at(a) > nt.GenPart_pt().at(b);
      });
    }
    else if (vdecays_idx.size() > 2)
    {
      std::cerr << "Error: more than 2 V daughters were found. This should never happen!" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    else
    {
      std::cerr << "Warning: Less than 2 V daughters were found." << std::endl;
    }

    // Determine if hadronic
    bool is_had = false; 
    for (int idx : vdecays_idx) {
      if (std::abs(nt.GenPart_pdgId().at(idx)) >= 1 && std::abs(nt.GenPart_pdgId().at(idx)) <= 6) {
        is_had = true;
        break;
      }
    }
    float firstV_pt = nt.GenPart_pt().at(firstV_idx);

    v_infos.push_back({firstV_idx, lastV_idx, vdecays_idx, is_had, firstV_pt});
  }

  // Sort: hadronic first, then by pt descending
  std::sort(v_infos.begin(), v_infos.end(), [](const VInfo& a, const VInfo& b) {
    int type_a = a.is_had ? 1 : 0;
    int type_b = b.is_had ? 1 : 0;
    if (type_a != type_b) {
      return type_a > type_b;
    } else {
      return a.first_pt > b.first_pt;
    }
  });

  // Set V bosons and their daughters
  for (size_t iV = 0; iV < v_infos.size(); ++iV)
  {
    const VInfo& info = v_infos[iV];
    int firstV_idx = info.first_idx;
    int firstV_pdgId = nt.GenPart_pdgId().at(firstV_idx);
    arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_pt", nt.GenPart_pt().at(firstV_idx));
    arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_eta", nt.GenPart_eta().at(firstV_idx));
    arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_phi", nt.GenPart_phi().at(firstV_idx));
    arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_mass", nt.GenPart_mass().at(firstV_idx));
    arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_pdgId", firstV_pdgId);
    arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(firstV_idx));
    arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_status", nt.GenPart_status().at(firstV_idx));
    arbusto.setLeaf<int>("gen_v" + std::to_string(iV + 1) + "_idx", firstV_idx);

    // Set V daughters variables
    for (size_t idaught = 0; idaught < info.daughters.size(); ++idaught)
    {
      int idaught_idx = info.daughters.at(idaught);
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_pt", nt.GenPart_pt().at(idaught_idx));
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_eta", nt.GenPart_eta().at(idaught_idx));
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_phi", nt.GenPart_phi().at(idaught_idx));
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_mass", nt.GenPart_mass().at(idaught_idx));
      arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_pdgId", nt.GenPart_pdgId().at(idaught_idx));
      arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(idaught_idx));
      arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_status", nt.GenPart_status().at(idaught_idx));
      arbusto.setLeaf<int>("gen_v" + std::to_string(iV + 1) + "q" + std::to_string(idaught + 1) + "_idx", idaught_idx);
    }
  }

  // Sort vbs quarks by pt
  if (vbsquarks_idx.size()==2) {
    if (cli.debug)
    {
      std::cout << "VBS jets before sorting " << std::endl;
      printTruthInfo(nt, vbsquarks_idx.at(0));
      printTruthInfo(nt, vbsquarks_idx.at(1));
    }

    std::sort(vbsquarks_idx.begin(), vbsquarks_idx.end(), [this](int a, int b) {
      return nt.GenPart_pt().at(a) > nt.GenPart_pt().at(b);
    });
  }

  // Set vbs quarks variables
  for (size_t ivbsj = 0; ivbsj < vbsquarks_idx.size(); ++ivbsj)
  {
    int ivbsj_idx = vbsquarks_idx.at(ivbsj);
    if (cli.debug)
    {
      std::cout << "Found VBS jet " << ivbsj+1 << std::endl;
      printTruthInfo(nt, ivbsj_idx);
    }
    arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_pt", nt.GenPart_pt().at(ivbsj_idx));
    arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_eta", nt.GenPart_eta().at(ivbsj_idx));
    arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_phi", nt.GenPart_phi().at(ivbsj_idx));
    arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_mass", nt.GenPart_mass().at(ivbsj_idx));
    arbusto.setLeaf<int>("truthVBSq" + std::to_string(ivbsj + 1) + "_pdgId", nt.GenPart_pdgId().at(ivbsj_idx));
    arbusto.setLeaf<int>("truthVBSq" + std::to_string(ivbsj + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(ivbsj_idx));
    arbusto.setLeaf<int>("truthVBSq" + std::to_string(ivbsj + 1) + "_status", nt.GenPart_status().at(ivbsj_idx));
    arbusto.setLeaf<int>("gen_vbs" + std::to_string(ivbsj + 1) + "_idx", ivbsj_idx);
  }
};


int findLastIndex(int current_Idx, int current_pdgId, Nano &nt)
{
  // std::cout << std::endl << "Entered: current_Idx = " << current_Idx << " , current_pdgId= " << current_pdgId << std::endl;
  int outIdx = current_Idx;
  for (size_t igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
  {
    int pdgId = nt.GenPart_pdgId().at(igen);
    int mother_idx = nt.GenPart_genPartIdxMother().at(igen);
    // if it's the daughter of the current particle
    if (mother_idx == current_Idx)
    {
      // if it's still the same particle, find the next in the chain
      if (pdgId == current_pdgId)
      {
        outIdx = findLastIndex(igen, pdgId, nt);
      }
      else
      {
        // std::cout << "Leaving outIdx = current_Idx" << std::endl;
      }
    }
  }
  return outIdx;
};

void printTruthInfo(Nano &nt, int igen)
{
  std::streamsize p = std::cout.precision();
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "    IDX:" << std::setw(4) << igen;
  std::cout << "    PDG:" << std::setw(5) << nt.GenPart_pdgId().at(igen) << std::setw(10) << std::left;
  std::cout << std::right;
  std::cout << "    STATUS:" << std::setw(3) << nt.GenPart_status().at(igen);
  std::cout << "    PtEtaPhiM:("
            << std::setw(8) << nt.GenPart_pt().at(igen) << ", "
            << std::setw(8) << nt.GenPart_eta().at(igen) << ", "
            << std::setw(8) << nt.GenPart_phi().at(igen) << ", "
            << std::setw(8) << nt.GenPart_mass().at(igen) << ")";
  std::cout << "    mothIDX: " << nt.GenPart_genPartIdxMother().at(igen) << std::endl;
  std::cout.unsetf(std::ios::floatfield);
  std::cout.precision(p);
};

void dumpTruthEventInfo(Nano &nt)
{
  for (size_t igen = 0; igen < nt.GenPart_pdgId().size(); ++igen)
  {
    printTruthInfo(nt, igen);
  }
};

bool isQuark(int pdgId)
{
  return (abs(pdgId) == 1) || (abs(pdgId) == 2) || (abs(pdgId) == 3) || (abs(pdgId) == 4) || (abs(pdgId) == 5) || (abs(pdgId) == 6);
};

};

#endif