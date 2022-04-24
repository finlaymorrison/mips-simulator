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
}

void Fetch::set_pc(const Bits<32>& new_pc_value)
{
    std::cout << "set pc:" << bit_str<32>(new_pc_value) << std::endl;
    PC = new_pc_value;
}

InterstageReg Fetch::run()
{

    InterstageReg reg;
    reg.reg_a_data = empty_bits<32>();
    reg.reg_b_data = empty_bits<32>();
    reg.reg_c_data = empty_bits<32>();
    reg.instruction = fetch_instruction();
    reg.instruction_addr = inc_word(inc_word(inc_word(inc_word(PC))));

    std::cout << "\tPC = " << bin_to_int<32>(PC) << " -> " << bit_str<32>(reg.instruction) << std::endl;

    return reg;
}

void Fetch::print_pc()
{
    std::cout << "PC : " << bit_str<32>(PC) << std::endl;
}