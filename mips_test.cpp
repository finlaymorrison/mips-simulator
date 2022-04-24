#define VERBOSE 1

#include "exceptions.h"
#include "mips.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>

/* 
 * reads text in file with the name provided into a vector of string of each line
 * filepath: path to source file
 * return: lines of input assembly in source file
 */
std::vector<std::string> read_file(std::string filepath)
{
    std::ifstream input(filepath);
    std::vector<std::string> lines;
    std::string str;
    while (std::getline(input, str))
    {
        // ignore empty lines
        if (str.size() > 0)
        {
            lines.push_back(str);
        }
    }
    return lines;
}

/* 
 * checks that arguments passed in on command line are valid
 * argc: number of arguments
 * argv: array of argument strings
 * return: path to assembly file
 */
std::pair<std::string, std::string> validate_args(int argc, char** argv)
{
    if (argc != 3)
    {
        throw ArgError("incorrect number of command line arguments");
    }
    if (!std::filesystem::exists(argv[1]))
    {
        throw ArgError("file does not exist");
    }
    return {argv[1], argv[2]};
}

std::vector<Bits<32>> parse_input(const std::vector<std::string>& input_data)
{
    std::vector<Bits<32>> out;
    for (const std::string line : input_data)
    {
        Bits<32> line_bin;
        for (int i = 0; i < line.size(); ++i)
        {
            if (line[i] == '0')
            {
                line_bin[i] = false;
            }
            else if (line[i] == '1')
            {
                line_bin[i] = true;
            }
            else
            {
                throw TestError("not a 0 or a 1");
            }
        }
        out.push_back(line_bin);
    }
    return out;
}

void write_binary(std::vector<std::string> data, std::string filepath)
{
    std::ofstream binary_file(filepath);
    for (const std::string& byte : data)
    {
        binary_file << byte << std::endl;
    }
}

/* 
 * main function of program
 * argc: number of arguments
 * argv: array of argument strings
 * return: error code
 */
int main(int argc, char** argv)
{
    try
    {
        std::vector<std::string> data = read_file(argv[1]);
        std::vector<Bits<32>> parsed = parse_input(data);
        MIPS mips(1000, 1000);
        mips.initialize_instruction_mem(parsed);
        mips.run_step();
        std::vector<std::string> mem_dump = mips.memory_dump();
        write_binary(mem_dump, argv[2]);
    }
    catch(const MemoryError& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        std::cerr << "memory error" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}