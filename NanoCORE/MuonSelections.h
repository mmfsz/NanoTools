#ifndef MUONSELECTIONS_H
#define MUONSELECTIONS_H
#include "Nano.h"
#include "Base.h"

namespace SS {
    bool muonID(unsigned int idx, SS::IDLevel id_level, int year);
    bool muon2016ID(unsigned int idx, SS::IDLevel id_level);
    bool muon2017ID(unsigned int idx, SS::IDLevel id_level);
    bool muon2018ID(unsigned int idx, SS::IDLevel id_level);
}

namespace ttH {
    bool muonID(unsigned int idx, ttH::IDLevel id_level, int year);
}
namespace ttH_UL {
    bool muonID(unsigned int idx, ttH::IDLevel id_level, int year);
}

namespace WWZ {
    bool muonID(int idx, WWZ::IDLevel id_level, int year);
    bool muon2016ID(unsigned int idx, WWZ::IDLevel id_level);
    bool muon2017ID(unsigned int idx, WWZ::IDLevel id_level);
    bool muon2018ID(unsigned int idx, WWZ::IDLevel id_level);
    bool muon2022ID(unsigned int idx, WWZ::IDLevel id_level);
}

#endif
