#include "writeback.h"

void Writeback::run(MEMWB memwb_reg, Decode& decode)
{
    int opcode_num = bin_to_int<6>(memwb_reg.opcode);

    switch(opcode_num)
    {
    case 7:
        /* lw */
        decode.set_register(memwb_reg.reg_dest_add, memwb_reg.write_data);
        break;
    case 10:
        /* jal */
        decode.set_register(memwb_reg.reg_dest_add, memwb_reg.write_data);
    default:
        /* do nothing */
        break;
    }
}