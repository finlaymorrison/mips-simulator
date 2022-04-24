#pragma once

#include "assembly.h"
#include "binary_ops.h"

/*
 * struct representing the registers between different parts of the pipeline
 */

struct InterstageReg
{
    Bits<32> instruction;
    Bits<32> instruction_addr;
    Bits<32> reg_a_data;
    Bits<32> reg_b_data;
    Bits<32> reg_c_data;
};

void clear(InterstageReg& reg);