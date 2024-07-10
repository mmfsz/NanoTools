#ifndef OBJECTSELECTION_BASE_H
#define OBJECTSELECTION_BASE_H

// RAPIDO
#include "arbol.h"
#include "arbusto.h"
#include "cutflow.h" // Cut
#include "hepcli.h"
#include "utilities.h" // Utilities::Variables

#include "Nano.h"

typedef std::vector<LorentzVector> LorentzVectors;
typedef std::vector<double> Doubles;
typedef std::vector<int> Integers;
typedef std::vector<unsigned int> Indices;

class ObjectSelection
{
  public:
    Arbusto& arbusto;
    Nano& nt;
    HEPCLI& cli;
    Utilities::Variables& globals;

    ObjectSelection(Arbusto &arbusto_ref, Nano &nt_ref, HEPCLI &cli_ref, Utilities::Variables &globals_ref)
        : arbusto(arbusto_ref),
          nt(nt_ref),
          cli(cli_ref),
          globals(globals_ref){
              // Do nothing
          };
};
#endif