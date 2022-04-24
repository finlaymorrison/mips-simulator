#include "decode.h"

Decode::Decode()
{
    for (Bits<32>& bits : registers)
    {
        for (bool& b : bits)
        {
            b = false;
        }
    }
}

size_t Decode::get_reg_add(const Bits<32>& instruction, int offset)
{
    int add = 0;
    for (int i = 0; i < 5; ++i)
    {
        add += instruction[offset+i] * std::pow(2, i);
    }
    return add;
}

Bits<16> Decode::get_const_data(const Bits<32>& instruction)
{
    Bits<16> const_data;
    for (int i = 0; i < 16; ++i)
    {
        const_data[i] = instruction[i+16];
    }
    return const_data;
}

Bits<6> Decode::get_op_data(const Bits<32>& instruction, int offset)
{
    Bits<6> op_data;
    for (int i = 0; i < 6; ++i)
    {
        op_data[i] = instruction[i+offset];
    }
    return op_data;
}

Bits<5> Decode::get_misc_data(const Bits<32>& instruction, int offset)
{
    Bits<5> op_data;
    for (int i = 0; i < 5; ++i)
    {
        op_data[i] = instruction[i+offset];
    }
    return op_data;
}

Bits<26> Decode::get_addr_const(const Bits<32>& instruction)
{
    Bits<26> op_data;
    for (int i = 0; i < 26; ++i)
    {
        op_data[i] = instruction[i+6];
    }
    return op_data;
}

InterstageReg Decode::run(const InterstageReg& ifid_reg)
{
    InterstageReg reg;

    reg.instruction = ifid_reg.instruction;
    reg.instruction_addr = ifid_reg.instruction_addr;

    size_t reg_a_add = get_reg_add(ifid_reg.instruction, 6);
    size_t reg_b_add = get_reg_add(ifid_reg.instruction, 11);
    size_t reg_c_add = get_reg_add(ifid_reg.instruction, 16);

    reg.reg_a_data = reg_a_add ? registers[reg_a_add] : Bits<32>();
    reg.reg_b_data = reg_b_add ? registers[reg_b_add] : Bits<32>();
    reg.reg_c_data = reg_c_add ? registers[reg_c_add] : Bits<32>();

    std::cout << '\t' << get_reg_name(reg_a_add) << " = " << bin_to_int<32>(reg.reg_a_data) << std::endl;
    std::cout << '\t' << get_reg_name(reg_b_add) << " = " << bin_to_int<32>(reg.reg_b_data) << std::endl;
    std::cout << '\t' << get_reg_name(reg_c_add) << " = " << bin_to_int<32>(reg.reg_c_data) << std::endl;

    return reg;
}

void Decode::set_register(const Bits<5>& addr, const Bits<32>& data)
{
    int addr_int = bin_to_int<5>(addr);
    registers[addr_int] = data;
}

void Decode::print_registers() const
{
    std::cout << "$zero : " << bin_to_int<32>(registers[0]) << std::endl;
    //std::cout << "$at   : " << bin_to_int<32>(registers[1]) << std::endl;
    std::cout << "$v0   : " << bin_to_int<32>(registers[2]) << std::endl;
    //std::cout << "$v1   : " << bin_to_int<32>(registers[3]) << std::endl;
    std::cout << "$a0   : " << bin_to_int<32>(registers[4]) << std::endl;
    std::cout << "$a1   : " << bin_to_int<32>(registers[5]) << std::endl;
    //std::cout << "$a2   : " << bin_to_int<32>(registers[6]) << std::endl;
    //std::cout << "$a3   : " << bin_to_int<32>(registers[7]) << std::endl;
    std::cout << "$t0   : " << bin_to_int<32>(registers[8]) << std::endl;
    //std::cout << "$t1   : " << bin_to_int<32>(registers[9]) << std::endl;
    //std::cout << "$t2   : " << bin_to_int<32>(registers[10]) << std::endl;
    //std::cout << "$t3   : " << bin_to_int<32>(registers[11]) << std::endl;
    //std::cout << "$t4   : " << bin_to_int<32>(registers[12]) << std::endl;
    //std::cout << "$t5   : " << bin_to_int<32>(registers[13]) << std::endl;
    //std::cout << "$t6   : " << bin_to_int<32>(registers[14]) << std::endl;
    //std::cout << "$t7   : " << bin_to_int<32>(registers[15]) << std::endl;
    std::cout << "$s0   : " << bin_to_int<32>(registers[16]) << std::endl;
    std::cout << "$s1   : " << bin_to_int<32>(registers[17]) << std::endl;
    //std::cout << "$s2   : " << bin_to_int<32>(registers[18]) << std::endl;
    //std::cout << "$s3   : " << bin_to_int<32>(registers[19]) << std::endl;
    //std::cout << "$s4   : " << bin_to_int<32>(registers[20]) << std::endl;
    //std::cout << "$s5   : " << bin_to_int<32>(registers[21]) << std::endl;
    //std::cout << "$s6   : " << bin_to_int<32>(registers[22]) << std::endl;
    //std::cout << "$s7   : " << bin_to_int<32>(registers[23]) << std::endl;
    //std::cout << "$t8   : " << bin_to_int<32>(registers[24]) << std::endl;
    //std::cout << "$t9   : " << bin_to_int<32>(registers[25]) << std::endl;
    //std::cout << "$k0   : " << bin_to_int<32>(registers[26]) << std::endl;
    //std::cout << "$k1   : " << bin_to_int<32>(registers[27]) << std::endl;
    //std::cout << "$gp   : " << bin_to_int<32>(registers[28]) << std::endl;
    std::cout << "$sp   : " << bin_to_int<32>(registers[29]) << std::endl;
    //std::cout << "$fp   : " << bin_to_int<32>(registers[30]) << std::endl;
    std::cout << "$ra   : " << bin_to_int<32>(registers[31]) << std::endl;
}