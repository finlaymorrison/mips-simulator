#pragma once

#include "assembly.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "mem_read.h"
#include "writeback.h"
#include "interstage_regs.h"

#include <vector>
#include <string>

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

    InterstageReg ifid_reg;
    InterstageReg idex_reg;
    InterstageReg exmem_reg;
    InterstageReg memwb_reg;
    bool inc;
    void check_pipeline(InterstageReg& ifid_n, InterstageReg& idex_n, InterstageReg& exmem_n, InterstageReg& memwb_n);
public:
    MIPS(size_t instruction_capacity, size_t data_capacity);
    void initialize_instruction_mem(const std::vector<Bits<32>>& instructions);
    int clock_cycle();
    void run_step();
    void run();

    std::vector<std::string> memory_dump() const;
};