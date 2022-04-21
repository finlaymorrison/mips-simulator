#pragma once

#include "exceptions.h"

#include <string>
#include <map>

/*
 * the type of a given instruction, InsType::NONE for an error
 */
enum class InsType
{
    NONE = 0,
    R,
    I,
    J
};

/*
 * takes in an opcode string and returns the integer value of the opcode in machine code
 * opcode_str: source asm opcode to find machine code for
 * return: machine code for passed in asm opcode
 */
int get_opcode(const std::string& opcode_str);

/*
 * takes in an opcode string and returns the instruction type of it
 * opcode_str: source asm opcode to find the type of
 * return: the instruction type of the instruction passed in
 */
InsType get_ins_type(const std::string& opcode_str);

/*
 * takes in an opcode string and returns the number of register operands it takes
 * opcode_str: source asm opcode to find the register operand count of
 * return: number of register operands the passed in asm opcode takes
 */
int get_reg_cnt(const std::string& opcode_str);

/*
 * takes in an opcode string and returns the nubmer of constant operands it takes
 * opcode_str: source asm opcode to find the constant operand count of
 * return: number of constant operands the passed in asm opcode takes
 */
int get_const_cnt(const std::string& opcode_str);

/*
 * takes in an opcode string and returns the alu function code of it
 * opcode_str: source asm opcode to find the aly funct of
 * return: alu funct of the passed in opcode
 */
int get_alu_funct(const std::string& opcode_str);

/*
 * takes in the asm name of a register and returns the address of the given register
 * reg_name: asm name of the register
 * return: address of the given register name
 */
int get_reg_address(const std::string& reg_name)