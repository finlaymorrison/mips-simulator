#include "writeback.h"

void Writeback::run(InterstageReg memwb_reg, Decode& decode)
{  
    Bits<5> dest_addr = extract_bits<32,5>(memwb_reg.instruction, 6);
    std::cout << '\t' << get_reg_name(bin_to_int<5>(dest_addr)) << " = " << bin_to_int<32>(memwb_reg.reg_b_data) << std::endl;
    /* if there is no write to make, register $zero is written to, so this can be unconditional */
    decode.set_register(dest_addr, memwb_reg.reg_b_data);
}