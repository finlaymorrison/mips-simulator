#include "execute.h"

std::pair<bool, Bits<32>> Execute::rtype(Bits<6> aluop, Bits<32> reg_a_data, Bits<32> reg_b_data, Bits<5> shamt)
{
    int aluop_num = bin_to_int<6>(aluop);
    Bits<32> res;
    switch (aluop_num)
    {
    case 0:
        /* addition */
        res = add_bin_nums<32>(reg_a_data, reg_b_data, false);
        break;
    case 1:
        /* subtraction */
        Bits<32> inv_b = bitwise_not<32>(reg_b_data);
        res = add_bin_nums<32>(reg_a_data, inv_b, true);
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
        Bits<32> or_val = bitwise_or<32>(reg_a_data, reg_b_data);
        res = bitwise_not<32>(or_val);
        break;
    case 5:
        /* bitwise nand */
        Bits<32> and_val = bitwise_and<32>(reg_a_data, reg_b_data);
        res = bitwise_not<32>(and_val);
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
    return {is_zero<32>(res), res};
}

EXMEM Execute::run(IDEX idex_reg)
{
    int opcode_num = bin_to_int<6>(idex_reg.opcode);

    EXMEM ret;

    switch (opcode_num)
    {
    case 0:
        /* r-type instruction */
        auto [zero, res] = rtype(idex_reg.aluop, idex_reg.reg_a_data, idex_reg.reg_b_data, idex_reg.shamt);
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    }
}