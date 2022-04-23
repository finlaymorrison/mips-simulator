#include "fetch.h"

Fetch::Fetch(size_t instruction_capacity) :
    instruction_mem(instruction_capacity)
{}

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
}

void Fetch::set_pc(const Bits<32>& new_pc_value)
{
    PC = new_pc_value;
}

IFID Fetch::run(EXMEM exmem_reg)
{
    IFID reg;
    inc_pc();
    reg.instruction = fetch_instruction();
    return reg;
}