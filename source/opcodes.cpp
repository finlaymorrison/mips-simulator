#include "opcodes.h"

int get_opcode(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",0}, {"sub",0}, {"and",0}, {"or", 0},
        {"nor", 0}, {"nand", 0}, {"srl", 0}, {"sll", 0},
        {"addi", 1}, {"andi", 2}, {"ori", 3}, {"bne", 4},
        {"beq", 5}, {"j", 6}
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
        {"beq", InsType::I}, {"j", InsType::J}
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

int get_reg_cnt(const std::string& opcode_str)
{
    static std::map<std::string, int> opcodes = {
        {"add",3}, {"sub",3}, {"and",3}, {"or", 3},
        {"nor", 3}, {"nand", 3}, {"srl", 2}, {"sll", 2},
        {"addi", 2}, {"andi", 2}, {"ori", 2}, {"bne", 2},
        {"beq", 2}, {"j", 0}
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
        {"beq", 1}, {"j", 1}
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
        {"nor", 4}, {"nand", 5}, {"srl", 6}, {"sll", 7}
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
