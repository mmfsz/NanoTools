
#include "vvhjj.h"


// If there aren't even two leptons that skip
bool runSelection_test( Nano& nt){
 
  if (not (nt.nElectron() + nt.nMuon() >= 2)) return false;
  std::cout << "1 " << std::endl;
  // Counter counting nveto leptons
  int nvetolep = 0;
  int nvetoelec = 0;
  int nvetomuon = 0;
  std::cout << "runSelection_test: nt.nElectron() " << nt.nElectron() << std::endl;
  // Loop over the electrons and count passing "veto"
  for (unsigned int iel = 0; iel < nt.nElectron(); ++iel)
  {
    std::cout << "iel " << iel << std::endl;
    std::cout << "WWZ::IDveto " << WWZ::IDveto << std::endl;
    std::cout << "nt.year() " << nt.year() << std::endl;

      if (WWZ::electronID(iel, WWZ::IDveto, nt.year())) {nvetoelec++;}
    std::cout << "nvetoelec " << nvetoelec << std::endl;
  }
  // Loop over the muons and count passing "veto"
  for (unsigned int imu = 0; imu < nt.nMuon(); ++imu)
  { 
   std::cout << "3 " << std::endl;
      if (WWZ::muonID(imu, WWZ::IDveto, nt.year())) nvetomuon++;
  }
  std::cout << "4 " << std::endl;
  nvetolep = nvetoelec + nvetomuon;
  if (not (nvetolep >= 2)) return false;
  std::cout << "5 " << std::endl;
  return true;
}

