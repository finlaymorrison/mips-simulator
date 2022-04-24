#include "execute.h"

Bits<32> Execute::rtype(Bits<6> aluop, Bits<32> reg_dest_data, Bits<32> reg_a_data, Bits<32> reg_b_data, Bits<5> shamt)
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
        res = reg_dest_data;
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

InterstageReg Execute::run(InterstageReg idex_reg)
{
    /* creating register to return and initializing its values */
    InterstageReg reg;
    reg.instruction = idex_reg.instruction;
    reg.instruction_addr = idex_reg.instruction_addr;
    reg.reg_a_data = idex_reg.reg_a_data;
    reg.reg_b_data = idex_reg.reg_b_data;
    reg.reg_c_data = idex_reg.reg_c_data;

    Bits<6> opcode = extract_bits<32,6>(idex_reg.instruction, 0);
    int opcode_num = bin_to_int<6>(opcode);

    switch (opcode_num)
    {
    case 0:{
        /* r-type instruction */
        Bits<6> aluop = extract_bits<32,6>(idex_reg.instruction, 26);
        Bits<5> shamt = extract_bits<32,5>(idex_reg.instruction, 21);
        reg.reg_b_data = rtype(aluop, idex_reg.reg_a_data, idex_reg.reg_b_data, idex_reg.reg_c_data, shamt);
        break;
    }
    case 1:{
        /* addi */
        Bits<32> const_data = sign_extend<16,32>(extract_bits<32,16>(idex_reg.instruction, 16));
        reg.reg_b_data = add_bin_nums<32>(idex_reg.reg_b_data, const_data, false);
        break;
    }
    case 2:{
        /* andi */
        Bits<32> const_data = sign_extend<16,32>(extract_bits<32,16>(idex_reg.instruction, 16));
        reg.reg_b_data = bitwise_and<32>(idex_reg.reg_b_data, const_data);
        break;
    }
    case 3:{
        /* ori */
        Bits<32> const_data = sign_extend<16,32>(extract_bits<32,16>(idex_reg.instruction, 16));
        reg.reg_b_data = bitwise_or<32>(idex_reg.reg_b_data, const_data);
        break;
    }
    case 4:{
        /* bne */
        Bits<32> not_reg_b = bitwise_not<32>(idex_reg.reg_b_data);
        reg.reg_b_data = add_bin_nums<32>(idex_reg.reg_a_data, not_reg_b, true);
        break;
    }
    case 5:{
        /* bne */
        Bits<32> not_reg_b = bitwise_not<32>(idex_reg.reg_b_data);
        reg.reg_b_data = add_bin_nums<32>(idex_reg.reg_a_data, not_reg_b, true);
        break;
    }
    case 6:{
        /* j */
        /* do nothing */
        break;
    }
    case 7:{
        /* lw */
        Bits<32> const_data = sign_extend<16,32>(extract_bits<32,16>(idex_reg.instruction, 16));
        reg.reg_b_data = add_bin_nums<32>(idex_reg.reg_b_data, const_data, false);
        break;
    }
    case 8:{
        /* sw */
        Bits<32> const_data = sign_extend<16,32>(extract_bits<32,16>(idex_reg.instruction, 16));
        reg.reg_b_data = add_bin_nums<32>(idex_reg.reg_b_data, const_data, false);
        break;
    }
    case 9:{
        /* subi */
        Bits<32> const_data = sign_extend<16,32>(extract_bits<32,16>(idex_reg.instruction, 16));
        Bits<32> not_const = bitwise_not<32>(const_data);
        reg.reg_b_data = add_bin_nums<32>(idex_reg.reg_b_data, not_const, true);
        break;
    }
    case 10:{
        /* jal */
        /* do nothing */
        break;
    }
    case 11:{
        /* exit */
        /* do nothing */
        break;
    }
    }

    std::cout << "\tres = " << bin_to_int<32>(reg.reg_b_data) << std::endl;

    return reg;
}