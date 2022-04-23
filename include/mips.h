#pragma once

#include "assembly.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "mem_read.h"
#include "writeback.h"
#include "interstage_regs.h"

#include <vector>

/*
 * controller of the CPU, instruction memory, and data memory
 */
class MIPS
{
private:
    int t_counter;
    Fetch fetch;
    Decode decode;
    Execute execute;
    MemRead mem_read;
    Writeback writeback;

    IFID ifid_reg;
    IDEX idex_reg;
    EXMEM exmem_reg;
    MEMWB memwb_reg;
public:
    MIPS(size_t instruction_capacity, size_t data_capacity);
    void initialize_instruction_mem(const std::vector<Bits<32>>& instructions);
    void clock_cycle();
    void run();
};