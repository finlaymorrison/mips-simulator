#pragma once

#include "interstage_regs.h"
#include "assembly.h"
#include "binary_ops.h"
#include "opcodes.h"

#include <cmath>

class Decode
{
private:
    std::array<Bits<32>, 32> registers;

    size_t get_reg_add(const Bits<32>& instruction, int offset);

    /* could use tempates with NTTAs but oh well */
    Bits<16> get_const_data(const Bits<32>& instruction);
    Bits<6> get_op_data(const Bits<32>& instruction, int offset);
    Bits<5> get_misc_data(const Bits<32>& instruction, int offset);
    Bits<26> get_addr_const(const Bits<32>& instruction);
public:
    Decode();
    
    InterstageReg run(const InterstageReg& ifid_reg);

    void set_register(const Bits<5>& addr, const Bits<32>& data);

    void print_registers() const;
};