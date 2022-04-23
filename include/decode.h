#pragma once

#include "interstage_regs.h"
#include "assembly.h"

#include <cmath>

class Decode
{
private:
    std::array<Bits<32>, 32> registers;

    size_t get_reg_add(const Bits<32>& instruction, int offset);

    /* could use tempates with NTTAs but its not worth the hassle */
    Bits<16> get_const_data(const Bits<32>& instruction);
    Bits<6> get_op_data(const Bits<32>& instruction, int offset);
    Bits<5> get_misc_data(const Bits<32>& instruction, int offset);
    Bits<26> get_addr_const(const Bits<32>& instruction);
public:
    Decode() = default;
    
    IDEX run(IFID ifid_reg, MEMWB memwb_reg);
};