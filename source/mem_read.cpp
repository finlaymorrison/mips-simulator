#include "mem_read.h"

MemRead::MemRead(size_t data_capacity) :
    data_memory(data_capacity)
{}

MEMWB MemRead::run(EXMEM exmem_reg, Fetch& fetch)
{
    int opcode_num = bin_to_int<6>(exmem_reg.opcode);
    int alu_opcode_num = bin_to_int<6>(exmem_reg.aluop);

    MEMWB ret;
    ret.opcode = exmem_reg.opcode;

    ret.reg_dest_add = {false, false, false ,false, false};

    switch(opcode_num)
    {
    case 0:
        if (alu_opcode_num == 8)
        {
            /* jr */
            fetch.set_pc(exmem_reg.alures);
        }
        else
        {
            ret.write_data = exmem_reg.alures;
            ret.reg_dest_add = exmem_reg.reg_dest_add;
        }
        
        break;
    case 1:
    case 2:
    case 3:
    case 9:
        /* addi, andi, ori, subi */
        ret.write_data = exmem_reg.alures;
        ret.reg_dest_add = exmem_reg.reg_dest_add;
        break;
    case 4:
        /* bne */
        if (!exmem_reg.zero)
        {
            fetch.set_pc(exmem_reg.br_addr);
        }
        break;
    case 5:
        /* beq */
        if (exmem_reg.zero)
        {
            fetch.set_pc(exmem_reg.br_addr);
        }
        break;
    case 6:
        /* j */
        fetch.set_pc(exmem_reg.addr);
        break;
    case 7:
        /* lw */
        ret.write_data = data_memory.read_word(exmem_reg.alures);
        ret.reg_dest_add = exmem_reg.reg_dest_add;
        break;
    case 8:
        /* sw */
        data_memory.write_word(exmem_reg.alures, exmem_reg.reg_dest_data);
        break;
    case 10:
        /* jal */
        fetch.set_pc(exmem_reg.addr);
        ret.write_data = exmem_reg.instruction_addr;
        ret.reg_dest_add = {true,true,true,true,true};
    default:
        /* do nothing */
        break;
    }

    return ret;
}

std::vector<std::string> MemRead::memory_dump() const
{
    return data_memory.memory_dump();
}