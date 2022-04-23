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
    std::vector<Bits<8>> buffer;
public:
    Memory(size_t mem_size);

    /*
     * functions used to manipulate memory when running the emulator, the addresses are
     * converted to use size_t and as such these functions are implemented using the
     * set of functions below this one
     */
    void write_word(const Bits<32>& location, const Bits<32>& data);
    void write_byte(const Bits<32>& location, const Bits<8>& data);
    Bits<32> read_word(const Bits<32>& location) const;
    Bits<8> read_byte(const Bits<32>& location) const;

    /*
     * functions are used to initialize the memory before the emulator is started, and
     * is also used internally when the emulator is running after converting from the 
     * bit representation to an integer representation.
     */
    void write_word(size_t location, const Bits<32>& data);
    void write_byte(size_t location, const Bits<8>& data);
    Bits<32> read_word(size_t location) const;
    Bits<8> read_byte(size_t location) const;
};

size_t convert_bits(const Bits<32>& binary);