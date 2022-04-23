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
    Bits<32> reg_a_data;
    Bits<32> reg_b_data;
    Bits<32> const_data;
};

struct EXMEM
{

};

struct MEMWB
{

};