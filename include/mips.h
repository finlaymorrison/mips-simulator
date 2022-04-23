#pragma once

#include "assembly.h"
#include "mmemory.h"
#include "cpu.h"

#include <vector>

/*
 * controller of the CPU, instruction memory, and data memory
 */
class MIPS
{
private:
    CPU cpu;
    Memory instruction_memory;
    Memory data_memory;
public:
    MIPS() = default;
};