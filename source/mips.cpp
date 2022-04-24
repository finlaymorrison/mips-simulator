#include "mips.h"

#include <array>

MIPS::MIPS(size_t instruction_capacity, size_t data_capacity) :
    fetch(instruction_capacity), mem_read(data_capacity), t_counter(0)
{
    clear(ifid_reg);
    clear(idex_reg);
    clear(exmem_reg);
    clear(memwb_reg);
}

void MIPS::initialize_instruction_mem(const std::vector<Bits<32>>& instructions)
{
    fetch.write_instructions(instructions);
}

int MIPS::clock_cycle()
{
    if (t_counter % 5 == 0)
    {
        std::cout << "fetch" << std::endl;
        InterstageReg ifid_n = fetch.run();
        ifid_reg = ifid_n;
    }
    if (t_counter % 5 == 1)
    {
        std::cout << "decode" << std::endl;
        InterstageReg idex_n = decode.run(ifid_reg);
        idex_reg = idex_n;
    }
    if (t_counter % 5 == 2)
    {
        std::cout << "execute" << std::endl;
        InterstageReg exmem_n = execute.run(idex_reg);
        exmem_reg = exmem_n;
    }
    if (t_counter % 5 == 3)
    {
        std::cout << "memread" << std::endl;
        InterstageReg memwb_n = mem_read.run(exmem_reg, fetch);
        memwb_reg = memwb_n;
    }
    if (t_counter % 5 == 4)
    {
        std::cout << "writeback" << std::endl;
        writeback.run(memwb_reg, decode);
    }
    ++t_counter;
    Bits<6> opcode = extract_bits<32,6>(exmem_reg.instruction, 0);
    if (bin_to_int<6>(opcode) == 11)
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
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
    }
}

void MIPS::run()
{
    for (;;)
    {
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
    }
}

std::vector<std::string> MIPS::memory_dump() const
{
    return mem_read.memory_dump();
}