#pragma once

#include "interstage_regs.h"
#include "binary_ops.h"

#include <utility>

class Execute
{
private:
    Bits<32> rtype(Bits<6> aluop, Bits<32> reg_a_data, Bits<32> reg_b_data, Bits<5> shamt);
public:
    Execute() = default;

    EXMEM run(IDEX idex_reg);
};