#pragma once

#include "assembly.h"

#include <vector>
#include <algorithm>

#include <cmath>

/*
 * byte addressed block of memory used for the instruction and data memory
 */
class Memory
{
private:
    size_t convert_bits(const Bits<32>& binary) const;
    void write_word(size_t location, const Bits<32>& data);
    void write_byte(size_t location, const Bits<8>& data);
    Bits<32> read_word(size_t location) const;
    Bits<8> read_byte(size_t location) const;
    std::vector<Bits<8>> buffer;
public:
    Memory(size_t mem_size);
    void write_word(const Bits<32>& location, const Bits<32>& data);
    void write_byte(const Bits<32>& location, const Bits<8>& data);
    Bits<32> read_word(const Bits<32>& location) const;
    Bits<8> read_byte(const Bits<32>& location) const;
};