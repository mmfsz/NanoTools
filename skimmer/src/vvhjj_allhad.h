#ifndef VVHJJ_ALLHAD_H
#define VVHJJ_ALLHAD_H

// Nano
#include "Nano.h"

//#include "arbol.h"
// #include "looper.h"
 #include "cutflow.h"
// #include "utilities.h"

// Custom skimmer 
#include "mvaTTH.h"

//ROOT
#include "Math/VectorUtil.h"

typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;


//bool runSelection_allHad( Nano& nt) ; //,  Arbusto& arbusto); 
//void computeLeptonMVA_ttHUL(Nano& nt, Arbusto& arbusto);
bool passLeptonVeto(Nano &nt);
bool runJetsSelection(Nano &nt);
bool runJetsSelection_Run2(Nano &nt);
LorentzVectors selectAK4Jets(Nano &nt);
bool runVBSSelection(LorentzVectors);
LorentzVectors selectAK8Jets(Nano &nt);

#endif