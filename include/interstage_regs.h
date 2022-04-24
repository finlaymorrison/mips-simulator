#pragma once

#include "assembly.h"

/*
 * structs representing the registers between different parts of the pipeline
 */
struct IFID
{
    Bits<32> instruction;

    Bits<32> instruction_addr;
};

struct IDEX
{
    Bits<6> opcode;
    Bits<6> aluop;
    Bits<5> shamt;
    Bits<5> reg_a_add;
    Bits<5> reg_b_add;
    Bits<5> reg_dest_add;
    Bits<26> addr;

    Bits<32> reg_a_data;
    Bits<32> reg_b_data;
    Bits<32> const_data;

    Bits<32> instruction_addr;

    /* for sw instruction */
    Bits<32> reg_dest_data;
};

struct EXMEM
{
    Bits<6> opcode;
    Bits<5> reg_dest_add;

    Bits<32> br_addr;

    bool zero;
    Bits<32> alures;
    Bits<32> addr;
    Bits<6> aluop;

    Bits<32> instruction_addr;

    /* for sw instruction */
    Bits<32> reg_dest_data;
};

struct MEMWB
{
    Bits<32> write_data;
    Bits<5> reg_dest_add;
    Bits<6> opcode;
};

struct InterstageReg
{
    Bits<32> instruction;
    Bits<32> reg_a;
    Bits<32> reg_b;
    Bits<32> reg_c;
};

MEMWB empty_memwb();