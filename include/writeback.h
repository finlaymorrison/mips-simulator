#pragma once

#include "interstage_regs.h"
#include "decode.h"

class Writeback
{
private:
public:
    Writeback() = default;

    void run(MEMWB memwb_reg, Decode& decode);
};