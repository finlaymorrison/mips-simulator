#include "execute.h"

Bits<32> Execute::rtype(Bits<6> aluop, Bits<32> reg_a_data, Bits<32> reg_b_data, Bits<5> shamt)
{
    int aluop_num = bin_to_int<6>(aluop);
    Bits<32> res;

    Bits<32> tmp;
    switch (aluop_num)
    {
    case 0:
        /* addition */
        res = add_bin_nums<32>(reg_a_data, reg_b_data, false);
        break;
    case 1:
        /* subtraction */
        tmp = bitwise_not<32>(reg_b_data);
        res = add_bin_nums<32>(reg_a_data, tmp, true);
        break;
    case 2:
        /* bitwise and */
        res = bitwise_and<32>(reg_a_data, reg_b_data);
        break;
    case 3:
        /* bitwise or */
        res = bitwise_or<32>(reg_a_data, reg_b_data);
        break;
    case 4:
        /* bitwise nor */
        tmp = bitwise_or<32>(reg_a_data, reg_b_data);
        res = bitwise_not<32>(tmp);
        break;
    case 5:
        /* bitwise nand */
        tmp = bitwise_and<32>(reg_a_data, reg_b_data);
        res = bitwise_not<32>(tmp);
        break;
    case 6:
        /* right shift */
        res = right_shift<32>(reg_a_data, shamt);
        break;
    case 7:
        /* left shift  */
        res = left_shift<32>(reg_a_data, shamt);
        break;
    case 8:
        /* jump register */
        res = reg_a_data;
        break;
    case 9:
        /* set greater than */
        res = sgt<32>(reg_a_data, reg_b_data);
        break;
    case 10:
        /* set less than */
        res = slt<32>(reg_a_data, reg_b_data);
        break;
    }
    return res;
}

EXMEM Execute::run(IDEX idex_reg)
{
    int opcode_num = bin_to_int<6>(idex_reg.opcode);

    EXMEM ret;
    ret.opcode = idex_reg.opcode;
    ret.reg_dest_add = idex_reg.reg_dest_add;
    ret.addr = sign_extend<26,32>(idex_reg.addr);
    ret.br_addr = idex_reg.const_data;
    ret.aluop = idex_reg.aluop;
    ret.instruction_addr = idex_reg.instruction_addr;
    ret.reg_dest_data = idex_reg.reg_dest_data;

    Bits<32> tmp;
    switch (opcode_num)
    {
    case 0:
        /* r-type instruction */
        ret.alures = rtype(idex_reg.aluop, idex_reg.reg_a_data, idex_reg.reg_b_data, idex_reg.shamt);
        break;
    case 1:
        /* addi */
        ret.alures = add_bin_nums<32>(idex_reg.reg_a_data, idex_reg.const_data, false);
        break;
    case 2:
        /* andi */
        ret.alures = bitwise_and<32>(idex_reg.reg_a_data, idex_reg.const_data);
        break;
    case 3:
        /* ori */
        ret.alures = bitwise_or<32>(idex_reg.reg_a_data, idex_reg.const_data);
        break;
    case 4:
        /* bne */
        tmp = bitwise_not<32>(idex_reg.reg_b_data);
        ret.alures = add_bin_nums<32>(idex_reg.reg_a_data, tmp, true);
        break;
    case 5:
        /* beq */
        tmp = bitwise_not<32>(idex_reg.reg_b_data);
        ret.alures = add_bin_nums<32>(idex_reg.reg_a_data, tmp, true);
        break;
    case 6:
        /* j */
        /* not required for j instruction, so setting to this as default */
        ret.alures = idex_reg.reg_a_data;
        break;
    case 7:
        /* lw */
        ret.alures = add_bin_nums<32>(idex_reg.reg_a_data, idex_reg.const_data, false);
        break;
    case 8:
        /* sw */
        ret.alures = add_bin_nums<32>(idex_reg.reg_a_data, idex_reg.const_data, false);
        break;
    case 9:
        /* subi */
        tmp = bitwise_not<32>(idex_reg.const_data);
        ret.alures = add_bin_nums<32>(idex_reg.reg_a_data, tmp, true);
        break;
    case 10:
        /* jal */
        ret.alures = idex_reg.reg_a_data;
        break;
    case 11:
        /* exit */
        /* not required for exit instruction, so setting to this as default */
        ret.alures = idex_reg.reg_a_data;
        break;
    }
    ret.zero = is_zero<32>(ret.alures);

    return ret;
}