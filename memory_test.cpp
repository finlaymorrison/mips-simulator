#include "exceptions.h"
#include "mmemory.h"
#include "assembly.h"

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

/*
 * parses the input file. two ints in a line, say a and b, mean a write to a with data b,
 * and one int in a line, say c, means a read from c.
 * cmds: raw list of memory operations
 * return: parsed list of memory operations
 */
std::vector<std::vector<Bits<32>>> parse_ops(std::vector<std::string> cmds)
{
    std::vector<std::string> cmds_clean = remove_whitespace(cmds);
    std::vector<std::vector<Bits<32>>> ops;
    for (const std::string& cmd : cmds_clean)
    {
        if (cmd.find(' ') == std::string::npos)
        {
            /* read operation */
            Bits<32> address;
            add_bits_to_word(address, std::stoi(cmd), 0, 32);
            ops.push_back({address});
        }
        else
        {
            int address = std::stoi(cmd.substr(0, cmd.find(' ')));
            int data = std::stoi(cmd.substr(cmd.find(' '), cmd.size() - cmd.find(' ')));
            Bits<32> address_bits;
            add_bits_to_word(address_bits, address, 0, 32);
            Bits<32> data_bits;
            add_bits_to_word(data_bits, data, 0, 32);
            ops.push_back({address_bits, data_bits});
        }
    }
    return ops;
}

/*
 * takes in a vector of test examples and runs then through the memory object passed in,
 * read operations have their output appended to the output vector
 * mem: memory object to perform operations on
 * test_points: vector of test examples
 * return: otuput of read operation tests
 */
std::vector<Bits<32>> run_test(Memory& mem, const std::vector<std::vector<Bits<32>>>& test_points)
{
    std::vector<Bits<32>> output;
    for (const std::vector<Bits<32>>& test_case : test_points)
    {
        if (test_case.size() == 1)
        {
            /* read operation */
            output.push_back(mem.read_word(test_case[0]));
        }
        else if (test_case.size() == 2)
        {
            /* write operation */
            mem.write_word(test_case[0], test_case[1]);
        }
        else
        {
            throw TestError("invalid number of arguments in test case");
        }
    }
    return output;
}

/*
 * writes output data from memory test to file provided
 * binary: memory output data
 * filepath: path to save memory data
 */
void write_binary(std::vector<Bits<32>> binary, std::string filepath)
{
    std::ofstream output_file(filepath);
    for (const Bits<32>& instruction : binary)
    {
        for (bool b : instruction)
        {
            output_file << b;
        }
        output_file << std::endl;
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
        auto [src, dest] = validate_args(argc, argv);
        std::vector<std::string> data = read_file(src);
        std::vector<std::vector<Bits<32>>> parsed = parse_ops(data);
        Memory mem(1000);
        std::vector<Bits<32>> output = run_test(mem, parsed);
        write_binary(output, dest);
    }
    catch(const TestError& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        std::cerr << "please check test format" << std::endl;
        return 1;
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}