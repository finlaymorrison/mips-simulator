#pragma once

#include "interstage_regs.h"
#include "mmemory.h"
#include "binary_ops.h"
#include "fetch.h"

#include <vector>
#include <string>

class MemRead
{
private:
    Memory data_memory;
public:
    MemRead(size_t data_capacity);

    InterstageReg run(InterstageReg exmem_reg, Fetch& fetch);
    std::vector<std::string> memory_dump() const;
};