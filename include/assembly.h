#pragma once

#include "exceptions.h"
#include "opcodes.h"

#include <vector>
#include <string>
#include <array>
#include <map>
#include <iostream>
#include <set>

/*
 * this datatype will be used heavily, so a simple type alias is used to make it more descriptive
 */
template<int width>
using Bits = std::array<bool, width>;

/* 
 * parses raw assembly code given as an argument into a vector of parsed instructions
 * lines: lines of raw input assembly
 * return: vector of parsed instrutions
 */
std::vector<Bits<32>> parse_asm(const std::vector<std::string>& lines);

/*
 * performs initial step of parsing asm by removing labels and unnessessary whitespace
 * raw_asm: raw asm file from source
 * return: pair of the cleaned asm code and a hashmap for the label to instruction address pairs
 */
std::pair<std::vector<std::string>, std::map<std::string, int>> prepare_asm(
    const std::vector<std::string>& raw_asm);

/*
 * parses an asm file once the labels have been removed and
 * unnessessary whitespace has been removed
 * lines: file with additional whitespace and labels removed
 * labels: hashmap of the label to instruction address pairs
 * return: vector of parsed instrutions
 */
std::vector<Bits<32>> parse_cleaned_asm(const std::vector<std::string>& lines, 
    const std::map<std::string, int>& labels);

/*
 * takes in the data of an ASM file and removes unnessessary whitespace, i.e. whitespace at the
 * ends of any line and more than one whitespace inbetween other characters, note that ',' is
 * also being treated as a whitespace by this function
 * lines: asm file to clean
 * return: cleaned asm file
 */
std::vector<std::string> remove_whitespace(const std::vector<std::string>& lines);

/*
 * takes in an asm file with its whitespace cleaned up and returns the label addresses
 * of each and every label in the asm file
 * lines: asm file with cleaned whitespace
 * return: hashmap of each label with its corresponding instruction address
 */
std::map<std::string, int> extract_labels(const std::vector<std::string>& lines);

/*
 * takes in an asm file with its whitespace cleaned up and removes all labels from
 * it, and removes the lines in which they resided on
 * lines: asm file with cleaned whitespace
 * return: asm file with labels removed
 */
std::vector<std::string> remove_labels(const std::vector<std::string>& lines);

/*
 * takes in the asm string of a single line of code along with
 * the label hashmap and parses it into its binary form
 * line: line of asm to be parsed
 * labels: hashmap of label to instruction address pairs
 * line_num: instruction address of the current instruction
 * return: parsed line of asm
 */
Bits<32> parse_asm_line(const std::string& line, const std::map<std::string, int>& labels, int line_num);

/*
 * takes in a line of asm with its operands split apart and substitutes instances of the labels in
 * line: asm line with no labels substituted
 * labels: hashmap of labels and their instruction addresses
 * return: asm line with labels substituded
 */
void substitute_labels(std::vector<std::string>& line,
    const std::map<std::string, int>& labels);

/*
 * each instruction of the three below have similar operation, and parse the asm instruction
 * passed in and put the machine code in the instruction word passed in, however each
 * instruction does this process for a specific type of instruction
 * line: single line of asm, each string represents on operand/operation name
 * line_num: instruction address of the current instruction
 * instruction_word: word to write binary version of asm to
 * return: whether or not the function successfully parsed the asm
 */
void parse_r_type(const std::vector<std::string>& line, Bits<32>& instruction_word);
void parse_i_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word);
void parse_j_type(const std::vector<std::string>& line, int line_num, Bits<32>& instruction_word);

/*
 * takes in a raw offset register operand, i.e `16($s3)` and returns
 * the offset integer and register address.
 * raw_offset: the raw offset register operand
 * return: pair of the offset and the offset register address
 */
std::pair<int, int> extract_offset(const std::string& raw_offset);

/*
 * returns whether the character passed in is a whitespace character (' ' or '\t')
 * c: character to check for whitespace
 * return: whether the character passed in is whitespace
 */
bool is_whitespace(char c);

/*
 * takes in the opcode string of an unparsed asm instruction and determines the instruction type
 * opcode_str: string of the opcode, i.e. "add", "addi", "j"
 * word: instruction word reference to fill in with the instructions opcode
 * return: type of the instruction, i.e. InsType::R, InsType::I
 */
InsType add_opcode(const std::string& opcode_str, Bits<32>& word);

/*
 * takes in a string and splits it into a vector of strings delimited by the value passed in
 * str: source string to split
 * delim: character to split in
 * return: split string
 */
std::vector<std::string> split_string(const std::string& str, char delim);

/*
 * takes in a word and a number to place in the word and write the bits of the number into the word
 * word: binary word to have the number placed inside
 * num: number to place inside the binary word
 * start: starting bit to place the number
 * len: bit width of number to write into word
 */
void add_bits_to_word(Bits<32>& word, int num, int start, int len);