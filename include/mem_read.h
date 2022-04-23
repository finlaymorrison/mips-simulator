#pragma once

#include "interstage_regs.h"

class MemRead
{
private:
    Memory data_memory;
public:
    MemRead(size_t data_capacity);

    MEMWB run(EXMEM exmem_reg);
};