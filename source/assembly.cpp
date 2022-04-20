#include "assembly.h"

bool is_whitespace(char c)
{
    return c == ' ' || c == '\t';
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

std::pair<std::vector<std::string>, std::map<std::string, int>> prepare_asm(const std::vector<std::string>& raw_asm)
{
    std::vector<std::string> rem_ws = remove_whitespace(raw_asm);
    std::map<std::string, int> labels = extract_labels(rem_ws);
    std::vector<std::string> cleaned_raw = remove_labels(rem_ws);
    return std::make_pair(cleaned_raw, labels);
}

std::vector<Bits<32>> parse_asm(std::vector<std::string> lines)
{
    auto [cleaned_lines, labels] = prepare_asm(lines);
    for (const std::string& line : cleaned_lines)
    {
        std::cout << line << std::endl;
    }
    return parse_cleaned_asm(cleaned_lines, labels);
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

Bits<32> parse_asm_line(std::string line, std::map<std::string, int> labels)
{
    std::vector<std::string> parts = split_string(line, ' ');
    return {};
}

std::vector<Bits<32>> parse_cleaned_asm(std::vector<std::string> lines, std::map<std::string, int> labels)
{
    std::vector<Bits<32>> parsed;
    for (const std::string& line : lines)
    {
        parsed.push_back(parse_asm_line(line, labels));
    }
    return parsed;
}