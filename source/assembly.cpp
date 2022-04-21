#include "assembly.h"

std::vector<Bits<32>> parse_asm(std::vector<std::string> lines)
{
    auto [cleaned_lines, labels] = prepare_asm(lines);
    return parse_cleaned_asm(cleaned_lines, labels);
}

std::pair<std::vector<std::string>, std::map<std::string, int>> prepare_asm(
    const std::vector<std::string>& raw_asm)
{
    std::vector<std::string> rem_ws = remove_whitespace(raw_asm);
    std::map<std::string, int> labels = extract_labels(rem_ws);
    std::vector<std::string> cleaned_raw = remove_labels(rem_ws);
    return std::make_pair(cleaned_raw, labels);
}

std::vector<Bits<32>> parse_cleaned_asm(std::vector<std::string> lines, 
    std::map<std::string, int> labels)
{
    std::vector<Bits<32>> parsed;
    int line_num = 0;
    for (const std::string& line : lines)
    {
        parsed.push_back(parse_asm_line(line, labels, line_num));
        ++line_num;
    }
    return parsed;
}

std::vector<std::string> remove_whitespace(const std::vector<std::string>& lines)
{
    std::vector<std::string> cleaned_code;
    for (const std::string& line : lines)
    {
        bool remove_next_whitespace = true;
        std::string cleaned_line;
        for (char c : line)
        {
            if (is_whitespace(c) || c == ',')
            {
                if (!remove_next_whitespace)
                {
                    cleaned_line += ' ';
                }
                remove_next_whitespace = true;
            }
            else
            {
                cleaned_line += c;
                remove_next_whitespace = false;
            }
        }
        if (is_whitespace(cleaned_line.back()))
        {
            cleaned_line.pop_back();
        }
        cleaned_code.push_back(cleaned_line);
    }
    return cleaned_code;
}

std::map<std::string, int> extract_labels(const std::vector<std::string>& lines)
{
    std::map<std::string, int> labels;
    std::vector<std::string>::iterator it;
    int line_num = 0;
    for (const std::string& line : lines)
    {
        if (line.back() == ':')
        {
            labels[line.substr(0, line.size()-1)] = line_num;
        }
        else
        {
            ++line_num;
        }
    }
    return labels;
}

std::vector<std::string> remove_labels(const std::vector<std::string>& lines)
{
    std::vector<std::string> labels_removed;
    for (const std::string& line : lines)
    {
        if (line.back() != ':')
        {
            labels_removed.push_back(line);
        }
    }
    return labels_removed;
}

Bits<32> parse_asm_line(std::string line, std::map<std::string, int> labels, int line_num)
{
    std::vector<std::string> parts = split_string(line, ' ');

    Bits<32> word;

    InsType ins_type = add_opcode(parts[0], word);
    bool success = false;
    switch (ins_type)
    {
    case InsType::R:
        success = parse_r_type(parts, line_num, word);
        break;
    case InsType::I:
        success = parse_i_type(parts, line_num, word);
        break;
    case InsType::J:
        success = parse_j_type(parts, line_num, word);
        break;
    case InsType::NONE:
        throw AsmError("invalid opcode string");
    }

    if (!success)
    {
        throw AsmError("cannot parse asm string");
    }

    return word;
}

/*
 * 6:opcode, 5:source_reg_a, 5:source_reg_b, 5:dest_reg, 5:shamt, 6:funct
 */
bool parse_r_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word)
{
    int index = 0;
    for (int i = 0; i < get_reg_cnt(line[0]); ++i)
    {

    }
    for (int i = 0; i < get_const_cnt(line[0]); ++i)
    {

    }
    add_alu_funct(instruction_word, line[0]);
    return true;
}

/*
 * 6:opcode, 5:source_reg, 5:dest_reg, 16:constant/address
 */
bool parse_i_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word)
{
    return true;
}

/*
 * 6:opcode, 26:address
 */
bool parse_j_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word)
{
    return true;
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

InsType add_opcode(std::string opcode_str, Bits<32>& word)
{
    int opcode = get_opcode(opcode_str);
    InsType ins_type = get_ins_type(opcode_str);
    add_bits_to_word(word, opcode, 0, 6);
    return ins_type;
}

bool is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}

std::vector<std::string> split_string(std::string str, char delim)
{
    std::vector<std::string> split = {""};
    for (char c : str)
    {
        if (c == delim)
        {
            split.push_back("");
        }
        else
        {
            split.back().push_back(c);
        }
    }
    if (split.back() == "")
    {
        split.pop_back();
    }
    return split;
}

void add_bits_to_word(Bits<32>& word, int num, int start, int len)
{
    for (int i = 0; i < len; ++i)
    {
        word[i + start] = num % 2;
        num >>= 1;
    }
}

void add_alu_funct(Bits<32>& word, const std::string& op_str)
{
    int alu_funct = get_alu_funct(op_str);
    add_bits_to_word(word, alu_funct, 26, 6);
}