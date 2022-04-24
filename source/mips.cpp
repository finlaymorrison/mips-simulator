#include "mips.h"

#include <array>

MIPS::MIPS(size_t instruction_capacity, size_t data_capacity) :
    fetch(instruction_capacity), mem_read(data_capacity), t_counter(0)
{
    ifid_reg = {};
    idex_reg = {};
    exmem_reg = {};
    memwb_reg = {};
}

void MIPS::initialize_instruction_mem(const std::vector<Bits<32>>& instructions)
{
    fetch.write_instructions(instructions);
}

bool is_jump_instruction(const Bits<6>& opcode, const Bits<6>& aluop)
{
    int opcode_str = bin_to_int<6>(opcode);
    return opcode_str == 4 || opcode_str == 5 || opcode_str == 6 ||
        opcode_str == 10 || (opcode_str == 0 && bin_to_int<6>(aluop) == 8);
}

int MIPS::clock_cycle()
{

    std::cout << "fetch" << std::endl;
    IFID ifid_n = fetch.run(exmem_reg);

    std::cout << "decode" << std::endl;
    IDEX idex_n = decode.run(ifid_reg);

    std::cout << "execute" << std::endl;
    EXMEM exmem_n = execute.run(idex_reg);

    std::cout << "memread" << std::endl;
    MEMWB memwb_n = mem_read.run(exmem_reg, fetch);

    std::cout << "writeback" << std::endl;
    writeback.run(memwb_reg, decode);

    /* checking whether to stall */
    if (is_jump_instruction(idex_reg.opcode, idex_reg.aluop))
    {
        ifid_reg = {};
        idex_reg = idex_n;
        exmem_reg = {};
        memwb_reg = empty_memwb();
        std::cout << "stall1" << std::endl;
        std::cout << memwb_reg.opcode[0] << std::endl;
    }
    else if (is_jump_instruction(exmem_reg.opcode, exmem_reg.aluop))
    {
        ifid_reg = {};
        idex_reg = {};
        exmem_reg = exmem_n;
        memwb_reg = empty_memwb();
        std::cout << "stall2" << std::endl;
    }
    else if (is_jump_instruction(memwb_reg.opcode, {false, false, false, false, false, false}))
    {
        ifid_reg = {};
        idex_reg = {};
        exmem_reg = {};
        memwb_reg = empty_memwb();
        std::cout << "stall3" << std::endl;
        std::cout << memwb_reg.opcode[0] << std::endl;
    }
    
    if (((bin_to_int<5>(idex_n.reg_a_add) != 0) && (bin_to_int<5>(exmem_n.reg_dest_add) == bin_to_int<5>(idex_n.reg_a_add) || bin_to_int<5>(memwb_n.reg_dest_add) == bin_to_int<5>(idex_n.reg_a_add))) ||
        ((bin_to_int<5>(idex_n.reg_b_add) != 0) && (bin_to_int<5>(exmem_n.reg_dest_add) == bin_to_int<5>(idex_n.reg_b_add) || bin_to_int<5>(memwb_n.reg_dest_add) == bin_to_int<5>(idex_n.reg_b_add))))
    {
        idex_reg = {};
        exmem_reg = exmem_n;
        memwb_reg = memwb_n;
        std::cout << "stall4" << std::endl;
    }
    else
    {
        ifid_reg = ifid_n;
        idex_reg = idex_n;
        exmem_reg = exmem_n;
        memwb_reg = memwb_n;
    }

    /* if exit instruction opcode, exit loop */
    if (bin_to_int<6>(memwb_reg.opcode) == 11)
    {
        return 1;
    }
    return 0;
}

void MIPS::run_step()
{
    for (;;)
    {
        std::cout << "press enter to step forward... ";
        std::cin.ignore();
        if(clock_cycle()) break;
    }
}

void MIPS::run()
{
    for (;;)
    {
        if(clock_cycle()) break;
    }
}

std::vector<std::string> MIPS::memory_dump() const
{
    return mem_read.memory_dump();
}