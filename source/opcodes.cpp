#include "opcodes.h"

int get_opcode(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",0}, {"sub",0}, {"and",0}, {"or", 0},
        {"nor", 0}, {"nand", 0}, {"srl", 0}, {"sll", 0},
        {"addi", 1}, {"andi", 2}, {"ori", 3}, {"bne", 4},
        {"beq", 5}, {"j", 6}, {"lw", 7}, {"sw", 8},
        {"subi", 9}, {"jr", 0}, {"jal", 10}, {"exit",11},
        {"sgt", 0}, {"slt", 0}
    };
    if (opcodes.find(opcode_str) != opcodes.end())
    {
        return opcodes[opcode_str];
    }
    else
    {
        throw AsmError("invalid opcode string");
    }
}

InsType get_ins_type(const std::string& opcode_str)
{
    static std::map<std::string, InsType> opcodes = {
        {"add", InsType::R}, {"sub", InsType::R},
        {"and", InsType::R}, {"or", InsType::R},
        {"nor", InsType::R}, {"nand", InsType::R},
        {"srl", InsType::R}, {"sll", InsType::R},
        {"addi", InsType::I}, {"andi", InsType::I},
        {"ori", InsType::I}, {"bne", InsType::I},
        {"lw", InsType::I}, {"sw", InsType::I},
        {"beq", InsType::I}, {"j", InsType::J},
        {"subi", InsType::I}, {"jr", InsType::R},
        {"jal", InsType::J}, {"exit", InsType::J},
        {"sgt", InsType::R}, {"slt", InsType::R}
    };
    if (opcodes.find(opcode_str) != opcodes.end())
    {
        return opcodes[opcode_str];
    }
    else
    {
        std::cout << opcode_str << std::endl;
        throw AsmError("invalid opcode string");
    }
}

int get_reg_cnt(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",3}, {"sub",3}, {"and",3}, {"or", 3},
        {"nor", 3}, {"nand", 3}, {"srl", 2}, {"sll", 2},
        {"addi", 2}, {"andi", 2}, {"ori", 2}, {"bne", 2},
        {"beq", 2}, {"j", 0}, {"lw", 1}, {"sw", 1},
        {"subi", 2}, {"jr", 1}, {"jal", 0}, {"exit", 0},
        {"sgt", 3}, {"slt", 3}
    };
    if (opcodes.find(opcode_str) != opcodes.end())
    {
        return opcodes[opcode_str];
    }
    else
    {
        throw AsmError("invalid opcode string");
    }
}

int get_const_cnt(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",0}, {"sub",0}, {"and",0}, {"or", 0},
        {"nor", 0}, {"nand", 0}, {"srl", 1}, {"sll", 1},
        {"addi", 1}, {"andi", 1}, {"ori", 1}, {"bne", 1},
        {"beq", 1}, {"j", 1}, {"lw", 0}, {"sw", 0},
        {"subi", 1}, {"jr", 0}, {"jal", 1}, {"exit", 0},
        {"sgt", 0}, {"slt", 0}
    };
    if (opcodes.find(opcode_str) != opcodes.end())
    {
        return opcodes[opcode_str];
    }
    else
    {
        throw AsmError("invalid opcode string");
    }
}

int get_offset_cnt(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",0}, {"sub",0}, {"and",0}, {"or", 0},
        {"nor", 0}, {"nand", 0}, {"srl", 0}, {"sll", 0},
        {"addi", 0}, {"andi", 0}, {"ori", 0}, {"bne", 0},
        {"beq", 0}, {"j", 0}, {"lw", 1}, {"sw", 1},
        {"subi", 0}, {"jr", 0}, {"jal", 0}, {"exit", 0},
        {"sgt", 0}, {"slt", 0}
    };
    if (opcodes.find(opcode_str) != opcodes.end())
    {
        return opcodes[opcode_str];
    }
    else
    {
        throw AsmError("invalid opcode string");
    }
}

int get_alu_funct(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",0}, {"sub",1}, {"and",2}, {"or", 3},
        {"nor", 4}, {"nand", 5}, {"srl", 6}, {"sll", 7},
        {"jr", 8}, {"sgt", 9}, {"slt", 10}
    };
    if (opcodes.find(opcode_str) != opcodes.end())
    {
        return opcodes[opcode_str];
    }
    else
    {
        throw AsmError("not an alu opcode");
    }
}

int get_reg_address(const std::string& reg_name)
{
    static std::map<std::string, int> reg_addresses = {
        {"$zero",0}, {"$at",1}, {"$v0",2}, {"$v1",3},
        {"$a0",4}, {"$a1",5}, {"$a2",6}, {"$a3",7},
        {"$t0",8}, {"$t1",9}, {"$t2",10}, {"$t3",11},
        {"$t4",12}, {"$t5",13}, {"$t6",14}, {"$t7",15},
        {"$s0",16}, {"$s1",17}, {"$s2",18}, {"$s3",19},
        {"$s4",20}, {"$s5",21}, {"$s6",22}, {"$s7",23},
        {"$t8",24}, {"$t9",25}, {"$k0",26}, {"$k1",27},
        {"$gp",28}, {"$sp",29}, {"$fp",30}, {"$ra",31},
    };
    if (reg_addresses.find(reg_name) != reg_addresses.end())
    {
        return reg_addresses[reg_name];
    }
    else
    {
        throw AsmError("invalid register name");
    }
}

std::string get_reg_name(int address)
{
    if (address > 31)
    {
        throw AsmError("invalid register address");
    }
    static std::array<std::string, 32> reg_names = {
        "$zero", "$at", "$v0", "$v1",
        "$a0", "$a1", "$a2", "$a3",
        "$t0", "$t1", "$t2", "$t3",
        "$t4", "$t5", "$t6", "$t7",
        "$s0", "$s1", "$s2", "$s3",
        "$s4", "$s5", "$s6", "$s7",
        "$t8", "$t9", "$k0", "$k1",
        "$gp", "$sp", "$fp", "$ra",
    };
    return reg_names[address];
}