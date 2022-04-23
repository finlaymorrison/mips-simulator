#pragma once

#include "interstage_regs.h"

class Execute
{
private:
public:
    Execute() = default;

    EXMEM run(IDEX idex_reg);
};