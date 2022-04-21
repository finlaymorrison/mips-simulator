#include "assembly.h"

std::vector<Bits<32>> parse_asm(const std::vector<std::string>& lines)
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

std::vector<Bits<32>> parse_cleaned_asm(const std::vector<std::string>& lines, 
    const std::map<std::string, int>& labels)
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

Bits<32> parse_asm_line(const std::string& line, const std::map<std::string, int>& labels, int line_num)
{
    std::vector<std::string> parts = split_string(line, ' ');

    substitute_labels(parts, labels);

    Bits<32> word;

    InsType ins_type = add_opcode(parts[0], word);
    bool success = false;
    switch (ins_type)
    {
    case InsType::R:
        parse_r_type(parts, word);
        break;
    case InsType::I:
        parse_i_type(parts, line_num, word);
        break;
    case InsType::J:
        parse_j_type(parts, line_num, word);
        break;
    case InsType::NONE:
        throw AsmError("invalid opcode string");
    }

    return word;
}

void substitute_labels(std::vector<std::string>& line,
    const std::map<std::string, int>& labels)
{
    for (std::string& arg : line)
    {
        if (labels.find(arg) != labels.end())
        {
            arg = std::to_string(labels.at(arg));
        }
    }
}

/*
 * 6:opcode, 5:source_reg_a, 5:source_reg_b, 5:dest_reg, 5:shamt, 6:funct
 */
void parse_r_type(const std::vector<std::string>& line, Bits<32>& instruction_word)
{
    if (get_reg_cnt(line[0]) == 3)
    {
        int reg_a_address = get_reg_address(line[1]);
        int reg_b_address = get_reg_address(line[2]);
        int reg_dest_address = get_reg_address(line[3]);
        add_bits_to_word(instruction_word, reg_a_address, 6, 5);
        add_bits_to_word(instruction_word, reg_b_address, 11, 5);
        add_bits_to_word(instruction_word, reg_dest_address, 16, 5);
        
    }
    else if (get_reg_cnt(line[0]) == 2)
    {
        int reg_a_address = get_reg_address(line[1]);
        int reg_dest_address = get_reg_address(line[2]);
        add_bits_to_word(instruction_word, reg_a_address, 6, 5);
        add_bits_to_word(instruction_word, 0, 11, 5);
        add_bits_to_word(instruction_word, reg_dest_address, 16, 5);
    }
    else
    {
        throw AsmError("invalid number of registers for an r-type instruction");
    }
    if (get_const_cnt(line[0]) == 1)
    {
        int shamt = std::stoi(line[2]);
        add_bits_to_word(instruction_word, shamt, 21, 5);
    }
    else if (get_const_cnt(line[0]) == 0)
    {
        add_bits_to_word(instruction_word, 0, 21, 5);
    }
    else
    {
        throw AsmError("invalid number of constants for an r-type instruction");
    }
    int alu_funct = get_alu_funct(line[0]);
    add_bits_to_word(instruction_word, alu_funct, 26, 6);
}

/*
 * 6:opcode, 5:reg_a, 5:reg_b, 16:constant/address
 */
void parse_i_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word)
{
    if (get_reg_cnt(line[0]) == 2)
    {
        int reg_a_address = get_reg_address(line[1]);
        int reg_b_address = get_reg_address(line[2]);
        add_bits_to_word(instruction_word, reg_a_address, 6, 5);
        add_bits_to_word(instruction_word, reg_b_address, 11, 5);
        if (get_const_cnt(line[0]) == 1)
        {
            // TODO: make relative branch
            add_bits_to_word(instruction_word, std::stoi(line[3]), 16, 16);
        }
        else
        {
            throw AsmError("invalid number of constants for an i-type instruction");
        }
    }
    else
    {
        throw AsmError("invalid number of registers for an i-type instruction");
    }
}

/*
 * 6:opcode, 26:address
 */
void parse_j_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word)
{
    if (get_reg_cnt(line[0]) == 0)
    {
        if (get_const_cnt(line[0]) == 1)
        {
            // TODO: use the strange stuff with the top 4 bits of pc and multiply by 4
            add_bits_to_word(instruction_word, std::stoi(line[1]), 6, 28);
        }
        else
        {
            throw AsmError("invalid number of constants for a j-type instruction");
        }
    }
    else
    {
        throw AsmError("invalid number of registers for a j-type instruction");
    }
}

InsType add_opcode(const std::string& opcode_str, Bits<32>& word)
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

std::vector<std::string> split_string(const std::string& str, char delim)
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