#ifndef LEPTON_TRUTH_ANALYSIS_H
#define LEPTON_TRUTH_ANALYSIS_H

// RAPIDO
#include "arbusto.h"
#include "hepcli.h"
// NanoCORE
#include "Nano.h"
#include "MCTools.h"

struct LeptonTruthAnalysis
{
  Arbusto &arbusto;
  Nano &nt;
  HEPCLI &cli;

  LeptonTruthAnalysis(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref)
      : arbusto(arbusto_ref), nt(nt_ref), cli(cli_ref){
        std::cout << "Initialize Lepton Truth Analysis object" << std::endl;
      };

  void initBranches()
  {
    arbusto.newVecBranch<int>("Muon_provenance");
    arbusto.newVecBranch<int>("Electron_provenance");
  };

  int getProvenance(int pdgId, int genPartIdx)
  {
    if (genPartIdx < 0) return -1;
    bool fromTau = isFromTau(pdgId, genPartIdx);
    if (fromTau && isFromW(pdgId, genPartIdx)) return 34;
    if (fromTau && isFromZ(pdgId, genPartIdx)) return 33;
    if (isFromW(pdgId, genPartIdx)) return 24;
    if (isFromZ(pdgId, genPartIdx)) return 23;
    if (isFromB(pdgId, genPartIdx)) return 5;
    if (isFromC(pdgId, genPartIdx)) return 4;
    if (isFromLight(pdgId, genPartIdx)) return 2;
    if (isFromLightFake(pdgId, genPartIdx)) return 1;
    return -1;
  };

  void setLeptonProvenance()
  {
    // Fill Muon_provenance
    for (unsigned int muon_i = 0; muon_i < nt.nMuon(); muon_i++)
    {
      int provenance = 0;
      if (!cli.is_data)
      {
        provenance = getProvenance(nt.Muon_pdgId()[muon_i], nt.Muon_genPartIdx()[muon_i]);
      }
      arbusto.appendToVecLeaf<int>("Muon_provenance", provenance);
    }

    // Fill Electron_provenance
    for (unsigned int elec_i = 0; elec_i < nt.nElectron(); elec_i++)
    {
      int provenance = 0;
      if (!cli.is_data)
      {
        provenance = getProvenance(nt.Electron_pdgId()[elec_i], nt.Electron_genPartIdx()[elec_i]);
      }
      arbusto.appendToVecLeaf<int>("Electron_provenance", provenance);
    }
  };
};

#endif
