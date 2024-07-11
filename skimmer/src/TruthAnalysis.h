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
    std::vector<float> hdecay_pt;
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
        else if ((status == 23) && (isQuark(pdgId)))
        {

          vbsquarks_idx.push_back(igen);
        }
      }
      else if (mother_idx > 0)
      {
        int mother_pdgId = nt.GenPart_pdgId().at(mother_idx);
        //  first daughters of last Higgs
        if ((mother_pdgId == 25) && (pdgId != 25))
        {
          if (cli.debug)
          {
            std::cout << "Found a Higgs daughter" << std::endl;
            printTruthInfo(nt, igen);
          }
          hdecay_idx.push_back(igen);
          hdecay_pt.push_back(nt.GenPart_pt().at(igen));
        }
      }
    }

    // Check nothing unexpected happened
    if (nFoundHiggs > 1)
    {
      std::cerr << "Error: found more than one Higgs. This should never happen!" << std::endl;
      dumpTruthEventInfo(nt);
      std::terminate();
    }
    else if ((nFoundVs > 2) && (vbsquarks_idx.size() == 2))
    {
      // some non-vbs events have 3 V bosons
      // e.g.
      //   IDX:   2    PDG:   23    STATUS: 22    PtEtaPhiM:(    91.2,     -2.2,     -0.9,    102.8)    mothIDX: 0
      //   IDX:   3    PDG:   23    STATUS: 22    PtEtaPhiM:(    67.8,     -2.1,     -0.2,     91.2)    mothIDX: 0
      //   IDX:   4    PDG:   23    STATUS: 22    PtEtaPhiM:(    67.2,     -2.3,      2.6,     91.2)    mothIDX: 0
      //   IDX:   5    PDG:   25    STATUS: 22    PtEtaPhiM:(    82.5,      0.7,      2.4,    125.0)    mothIDX: 0
      std::cerr << "Error: found more than two V's in a VBS event. This should never happen!" << std::endl;
      dumpTruthEventInfo(nt);
      std::terminate();
    }

    // do not set truth variables for events where some truth partons are missing
    if ((nFoundHiggs != 1) || (nFoundVs != 2))
    {
      std::cerr << "Warning: could not find exactly one Higgs, two V's. Truth variables will not be set for this event." << std::endl;
      // dumpTruthEventInfo(nt);
      return;
    }

    // If you get here, truth variables will be set. Some daughters and vbs jets might be missing.

    // ==========================================

    // Set Higgs variables
    arbusto.setLeaf<float>("truthH_pt", nt.GenPart_pt().at(higgs_idx));
    arbusto.setLeaf<float>("truthH_eta", nt.GenPart_eta().at(higgs_idx));
    arbusto.setLeaf<float>("truthH_phi", nt.GenPart_phi().at(higgs_idx));
    arbusto.setLeaf<float>("truthH_mass", nt.GenPart_mass().at(higgs_idx));
    arbusto.setLeaf<int>("truthH_pdgId", nt.GenPart_pdgId().at(higgs_idx));
    arbusto.setLeaf<int>("truthH_genPartIdxMother", nt.GenPart_genPartIdxMother().at(higgs_idx));
    arbusto.setLeaf<int>("truthH_status", nt.GenPart_status().at(higgs_idx));

    if (hdecay_pt.size() == 2)
    {
      if (hdecay_pt.at(0) < hdecay_pt.at(1))
      {
        std::swap(hdecay_pt.at(0), hdecay_pt.at(1));
        std::swap(hdecay_idx.at(0), hdecay_idx.at(1));
      }
    }
    else if (hdecay_pt.size() > 2)
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
    }

    // Set V bosons variables

    // sort V bosons by pt
    if (firstVs_pt.at(0) < firstVs_pt.at(1))
    {
      std::swap(firstVs_pt.at(0), firstVs_pt.at(1));
      std::swap(firstVs_idx.at(0), firstVs_idx.at(1));
    }

    for (size_t iV = 0; iV < firstVs_idx.size(); ++iV)
    {
      std::vector<int> vdecays_idx;
      std::vector<int> vdecays_pt;

      int firstV_idx = firstVs_idx.at(iV);
      int firstV_pdgId = nt.GenPart_pdgId().at(firstV_idx);
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_pt", nt.GenPart_pt().at(firstV_idx));
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_eta", nt.GenPart_eta().at(firstV_idx));
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_phi", nt.GenPart_phi().at(firstV_idx));
      arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_mass", nt.GenPart_mass().at(firstV_idx));
      arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_pdgId", firstV_pdgId);
      arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(firstV_idx));
      arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_status", nt.GenPart_status().at(firstV_idx));

      // Find last V boson in its decay chain
      int lastV_idx = findLastIndex(firstV_idx, firstV_pdgId, nt);

      // Find daughters of last V boson
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
          vdecays_pt.push_back(nt.GenPart_pt().at(igen));
        }
      }

      // Set V daughters variables
      if (vdecays_pt.size() == 2)
      {
        if (vdecays_pt.at(0) < vdecays_pt.at(1))
        {
          std::swap(vdecays_pt.at(0), vdecays_pt.at(1));
          std::swap(vdecays_idx.at(0), vdecays_idx.at(1));
        }
      }
      else if (vdecays_pt.size() > 2)
      {
        std::cerr << "Error: more than 2 V daughters were found. This should never happen!" << std::endl;
        std::exit(EXIT_FAILURE);
      }
      else
      {
        std::cerr << "Warning: Less than 2 V daughters were found." << std::endl;
      }

      for (size_t idaught = 0; idaught < vdecays_idx.size(); ++idaught)
      {
        int idaught_idx = vdecays_idx.at(idaught);
        arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_pt", nt.GenPart_pt().at(idaught_idx));
        arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_eta", nt.GenPart_eta().at(idaught_idx));
        arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_phi", nt.GenPart_phi().at(idaught_idx));
        arbusto.setLeaf<float>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_mass", nt.GenPart_mass().at(idaught_idx));
        arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_pdgId", nt.GenPart_pdgId().at(idaught_idx));
        arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(idaught_idx));
        arbusto.setLeaf<int>("truthV" + std::to_string(iV + 1) + "_daught" + std::to_string(idaught + 1) + "_status", nt.GenPart_status().at(idaught_idx));
      }
    }

    // Set vbs quarks variables
    for (size_t ivbsj = 0; ivbsj < vbsquarks_idx.size(); ++ivbsj)
    {
      int ivbsj_idx = vbsquarks_idx.at(ivbsj);
      arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_pt", nt.GenPart_pt().at(ivbsj_idx));
      arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_eta", nt.GenPart_eta().at(ivbsj_idx));
      arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_phi", nt.GenPart_phi().at(ivbsj_idx));
      arbusto.setLeaf<float>("truthVBSq" + std::to_string(ivbsj + 1) + "_mass", nt.GenPart_mass().at(ivbsj_idx));
      arbusto.setLeaf<int>("truthVBSq" + std::to_string(ivbsj + 1) + "_pdgId", nt.GenPart_pdgId().at(ivbsj_idx));
      arbusto.setLeaf<int>("truthVBSq" + std::to_string(ivbsj + 1) + "_genPartIdxMother", nt.GenPart_genPartIdxMother().at(ivbsj_idx));
      arbusto.setLeaf<int>("truthVBSq" + std::to_string(ivbsj + 1) + "_status", nt.GenPart_status().at(ivbsj_idx));
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