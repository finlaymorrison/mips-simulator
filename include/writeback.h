#pragma once

#include "interstage_regs.h"
#include "decode.h"

class Writeback
{
private:
public:
    Writeback() = default;

    void run(InterstageReg memwb_reg, Decode& decode);
};