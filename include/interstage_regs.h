#pragma once

#include "assembly.h"

/*
 * structs representing the registers between different parts of the pipeline
 */
struct IFID
{
    Bits<32> instruction;
};

struct IDEX
{
    Bits<6> opcode;
    Bits<6> aluop;
    Bits<5> shamt;
    Bits<5> reg_dest_add;
    Bits<26> addr;

    Bits<32> reg_a_data;
    Bits<32> reg_b_data;
    Bits<16> const_data;
};

struct EXMEM
{
    Bits<6> opcode;
    Bits<5> reg_dest_add;

    bool zero;
    Bits<32> alures;
    Bits<32> jmppc;
};

struct MEMWB
{

};