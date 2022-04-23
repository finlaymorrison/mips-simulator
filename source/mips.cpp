#include "mips.h"

#include <array>

MIPS::MIPS(size_t instruction_capacity, size_t data_capacity) :
    fetch(instruction_capacity), mem_read(data_capacity)
{}

void MIPS::initialize_instruction_mem(const std::vector<Bits<32>>& instructions)
{
    fetch.write_instructions(instructions);
}

void MIPS::clock_cycle()
{
    IFID ifid_n = fetch.run(exmem_reg);
    IDEX idex_n = decode.run(ifid_reg, memwb_reg);
    EXMEM exmem_n = execute.run(idex_reg);
    MEMWB memwb_n = mem_read.run(exmem_reg, fetch);
    writeback.run(memwb_n, decode);


    ifid_reg = ifid_n;
    idex_reg = idex_n;
    exmem_reg = exmem_n;
    memwb_reg = memwb_n;
}