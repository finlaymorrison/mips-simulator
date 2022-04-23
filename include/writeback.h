#pragma once

#include "interstage_regs.h"

class Writeback
{
private:
public:
    Writeback() = default;

    void run(MEMWB memwb_reg);
};