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
    ++t_counter;
    if (t_counter % 10 != 0)
    {
        return;
    }
    std::cout << "fetch" << std::endl;
    IFID ifid_n = fetch.run(exmem_reg);
    std::cout << "decode" << std::endl;
    IDEX idex_n = decode.run(ifid_reg, memwb_reg);
    std::cout << "execute" << std::endl;
    EXMEM exmem_n = execute.run(idex_reg);
    std::cout << "memread" << std::endl;
    MEMWB memwb_n = mem_read.run(exmem_reg, fetch);
    std::cout << "writeback" << std::endl;
    writeback.run(memwb_n, decode);


    ifid_reg = ifid_n;
    idex_reg = idex_n;
    exmem_reg = exmem_n;
    memwb_reg = memwb_n;
}

void MIPS::run()
{
    for (;;)
    {
        std::cout << "type a character to step through clock cycle: ";
        std::string dummy;
        std::cin >> dummy;
        clock_cycle();
    }
}