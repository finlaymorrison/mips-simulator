#include "mem_read.h"

MemRead::MemRead(size_t data_capacity) :
    data_memory(data_capacity)
{}

void set_write_addr(Bits<32>& instruction, const Bits<5>& addr)
{
    for (int i = 0; i < 5; ++i)
    {
        instruction[i+6] = addr[i];
    }
}

InterstageReg MemRead::run(InterstageReg exmem_reg, Fetch& fetch)
{
    InterstageReg reg;
    reg.instruction = exmem_reg.instruction;
    reg.instruction_addr = exmem_reg.instruction_addr;
    reg.reg_a_data = exmem_reg.reg_a_data;
    reg.reg_b_data = exmem_reg.reg_b_data;
    reg.reg_c_data = exmem_reg.reg_c_data;

    Bits<6> opcode = extract_bits<32,6>(exmem_reg.instruction, 0);
    int opcode_num = bin_to_int<6>(opcode);
    Bits<6> alu_opcode = extract_bits<32,6>(exmem_reg.instruction, 26);
    int alu_opcode_num = bin_to_int<6>(alu_opcode);

    switch(opcode_num)
    {
    case 0:
        if (alu_opcode_num == 8)
        {
            /* jr */
            fetch.set_pc(exmem_reg.reg_b_data);
            set_write_addr(reg.instruction, {false, false, false, false, false});
        }
        break;
    case 4:{
        /* bne */
        if (!is_zero<32>(exmem_reg.reg_b_data))
        {
            Bits<32> br_addr = sign_extend<16,32>(extract_bits<32,16>(exmem_reg.instruction, 16));
            fetch.set_pc(br_addr);
        }
        set_write_addr(reg.instruction, {false, false, false, false, false});
        break;
    }
    case 5:
        /* beq */
        if (is_zero<32>(exmem_reg.reg_b_data))
        {
            Bits<32> br_addr = sign_extend<16,32>(extract_bits<32,16>(exmem_reg.instruction, 16));
            fetch.set_pc(br_addr);
        }
        set_write_addr(reg.instruction, {false, false, false, false, false});
        break;
    case 6:{
        /* j */
        Bits<32> j_addr = sign_extend<26,32>(extract_bits<32,26>(exmem_reg.instruction, 6));
        fetch.set_pc(j_addr);
        set_write_addr(reg.instruction, {false, false, false, false, false});
        break;
    }
    case 7:
        /* lw */
        reg.reg_b_data = data_memory.read_word(exmem_reg.reg_b_data);
        break;
    case 8:
        /* sw */
        data_memory.write_word(exmem_reg.reg_b_data, exmem_reg.reg_a_data);
        set_write_addr(reg.instruction, {false, false, false, false, false});
        break;
    case 10:{
        /* jal */
        Bits<32> j_addr = sign_extend<26,32>(extract_bits<32,26>(exmem_reg.instruction, 6));
        fetch.set_pc(j_addr);
        reg.reg_b_data = exmem_reg.instruction_addr;
        set_write_addr(reg.instruction, {true, true, true, true, true});
    }
    default:
        /* do nothing */
        break;
    }

    return reg;
}

std::vector<std::string> MemRead::memory_dump() const
{
    return data_memory.memory_dump();
}