#pragma once

#include "mmemory.h"
#include "binary_ops.h"
#include "interstage_regs.h"

class Fetch
{
private:
    Memory instruction_mem;
    Bits<32> PC;
public:
    Fetch(size_t instruction_capacity);

    /*
     * functions used to set up fetch object before running emulator
     */
    void write_instructions(const std::vector<Bits<32>>& instructions);

    /*
     * functions used whilst running the emulator
     */
    void inc_pc();
    void set_pc(const Bits<32>& new_pc_value);
    Bits<32> fetch_instruction() const;
    IFID run(EXMEM exmem_reg);
};