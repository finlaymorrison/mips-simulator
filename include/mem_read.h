#pragma once

#include "interstage_regs.h"
#include "mmemory.h"

class MemRead
{
private:
    Memory data_memory;
public:
    MemRead(size_t data_capacity);

    MEMWB run(EXMEM exmem_reg);
};