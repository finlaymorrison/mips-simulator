#include "fetch.h"

Fetch::Fetch(size_t instruction_capacity) :
    instruction_mem(instruction_capacity)
{
    for (int i = 0; i < 32; ++i)
    {
        PC[i] = false;
    }
}

void Fetch::write_instructions(const std::vector<Bits<32>>& instructions)
{
    for (int i = 0; i < instructions.size(); ++i)
    {
        instruction_mem.write_word(i*4, instructions[i]);
    }
}

Bits<32> Fetch::fetch_instruction() const
{
    return instruction_mem.read_word(PC);
}

void Fetch::inc_pc()
{
    /* increment 4 times since memory is byte addressed */
    PC = inc_word(inc_word(inc_word(inc_word(PC))));
    int PC_int = bin_to_int<32>(PC);
}

void Fetch::set_pc(const Bits<32>& new_pc_value)
{
    PC = new_pc_value;
}

IFID Fetch::run(EXMEM exmem_reg)
{
    std::cout << "section a" << std::endl;
    inc_pc();

    IFID reg;

    std::cout << "section b" << std::endl;
    reg.instruction = fetch_instruction();
    reg.instruction_addr = PC;

    std::cout << "section c" << std::endl;
    return reg;
}