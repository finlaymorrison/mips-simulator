#include "interstage_regs.h"

void clear(InterstageReg& reg)
{
    reg.instruction = empty_bits<32>();
    reg.instruction_addr = empty_bits<32>();
    reg.reg_a_data = empty_bits<32>();
    reg.reg_b_data = empty_bits<32>();
    reg.reg_c_data = empty_bits<32>();
}