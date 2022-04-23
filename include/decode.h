#pragma once

#include "interstage_regs.h"

class Decode
{
private:
    std::array<Bits<32>, 32> registers;
public:
    Decode() = default;
    
    IDEX run(IFID ifid_reg, MEMWB memwb_reg);
};