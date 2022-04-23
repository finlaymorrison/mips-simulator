#include "mmemory.h"

Memory::Memory(size_t mem_size) :
    buffer(mem_size)
{}

size_t convert_bits(const Bits<32>& binary)
{
    size_t num = 0;
    for (int i = 0; i < binary.size(); ++i)
    {
        num += binary[i] * std::pow(2,i);
    }
    return num;
}

void Memory::write_word(size_t location, const Bits<32>& data)
{
    for (int i = 0; i < 4; ++i)
    {
        Bits<8> byte;
        std::copy(data.begin() + 8*i, data.begin() + 8 + 8*i, byte.begin());
        write_byte(location + i, byte);
    }
}

void Memory::write_byte(size_t location, const Bits<8>& data)
{
    buffer[location] = data;
}

Bits<32> Memory::read_word(size_t location) const
{
    Bits<32> data;
    for (int i = 0; i < 4; ++i)
    {
        Bits<8> byte = read_byte(location + i);
        std::copy(byte.begin(), byte.end(), data.begin() + 8*i);
    }
    return data;
}

Bits<8> Memory::read_byte(size_t location) const
{
    return buffer[location];
}

void Memory::write_word(const Bits<32>& location, const Bits<32>& data)
{
    write_word(convert_bits(location), data);
}

void Memory::write_byte(const Bits<32>& location, const Bits<8>& data)
{
    write_byte(convert_bits(location), data);
}

Bits<32> Memory::read_word(const Bits<32>& location) const
{
    return read_word(convert_bits(location));
}

Bits<8> Memory::read_byte(const Bits<32>& location) const
{
    return read_byte(convert_bits(location));
}