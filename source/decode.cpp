#include "decode.h"

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

IDEX Decode::run(IFID ifid_reg, MEMWB memwb_reg)
{
    size_t reg_a_add = get_reg_add(ifid_reg.instruction, 11);
    size_t reg_b_add = get_reg_add(ifid_reg.instruction, 16);
    Bits<16> const_data = get_const_data(ifid_reg.instruction);
    Bits<6> opcode = get_op_data(ifid_reg.instruction, 0);
    Bits<6> aluop = get_op_data(ifid_reg.instruction, 26);
    Bits<5> reg_dest_add = get_misc_data(ifid_reg.instruction, 6);
    Bits<5> shamt = get_misc_data(ifid_reg.instruction, 21);
    Bits<26> addr = get_addr_const(ifid_reg.instruction);

    IDEX idex_reg;

    idex_reg.reg_a_data = reg_a_add ? registers[reg_a_add] : Bits<32>();
    idex_reg.reg_b_data = reg_b_add ? registers[reg_b_add] : Bits<32>();
    idex_reg.const_data = sign_extend<16,32>(const_data);
    idex_reg.opcode = opcode;
    idex_reg.aluop = aluop;
    idex_reg.reg_dest_add = reg_dest_add;
    idex_reg.shamt = shamt;
    idex_reg.addr = addr;
    idex_reg.instruction_addr = ifid_reg.instruction_addr;

    /* for sw instruction */
    size_t reg_dest_add_num = get_reg_add(ifid_reg.instruction, 6);
    idex_reg.reg_dest_data = registers[reg_dest_add_num];

    return idex_reg;
}

void Decode::set_register(const Bits<5>& addr, const Bits<32>& data)
{
    int addr_int = bin_to_int<5>(addr);
    registers[addr_int] = data;
}