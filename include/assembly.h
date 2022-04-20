#include <vector>
#include <string>
#include <array>
#include <map>
#include <iostream>

template<int width>
using Bits = std::array<bool, width>;

bool is_whitespace(char c);

std::map<std::string, int> extract_labels(const std::vector<std::string>& lines);

std::vector<std::string> remove_whitespace(const std::vector<std::string>& lines);

std::vector<std::string> remove_labels(const std::vector<std::string>& lines);

/* 
 * parses raw assembly code given as an argument into a vector of parsed instructions
 * lines: lines of raw input assembly
 * return: vector of parsed instrutions
 */
std::vector<Bits<32>> parse_asm(std::vector<std::string> lines);

std::vector<Bits<32>> parse_cleaned_asm(std::vector<std::string> lines, std::map<std::string, int> labels);

std::vector<std::string> split_string(std::string str, char delim);

Bits<32> parse_asm_line(std::string line, std::map<std::string, int> labels);