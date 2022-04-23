#include "writeback.h"

void Writeback::run(MEMWB memwb_reg, Decode& decode)
{
    int opcode_num = bin_to_int<6>(memwb_reg.opcode);
    
    std::cout << '\t' << get_reg_name(bin_to_int<5>(memwb_reg.reg_dest_add)) << " = " << bin_to_int<32>(memwb_reg.write_data) << std::endl;

    /* if there is no write to make, register $zero is written to, so this can be unconditional */
    decode.set_register(memwb_reg.reg_dest_add, memwb_reg.write_data);
}