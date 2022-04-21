#include "assembly.h"
#include "exceptions.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>

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
 * writes an assembled binary file to a filename with the path given
 * binary: assembled binary data
 * filepath: path to save binary data
 */
void write_binary(std::vector<Bits<32>> binary, std::string filepath)
{
    std::ofstream binary_file(filepath);
    for (const Bits<32>& instruction : binary)
    {
        for (bool b : instruction)
        {
            binary_file << b;
        }
        binary_file << std::endl;
    }
}

/* 
 * checks that arguments passed in on command line are valid
 * argc: number of arguments
 * argv: array of argument strings
 * return: path to assembly file
 */
std::string validate_args(int argc, char** argv)
{
    if (argc != 2)
    {
        throw ArgError("incorrect number of command line arguments");
    }
    if (!std::filesystem::exists(argv[1]))
    {
        throw ArgError("file does not exist");
    }
    return argv[1];
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
        std::string source_file = validate_args(argc, argv);
        std::vector<std::string> source_lines = read_file(source_file);
        std::vector<Bits<32>> instructions = parse_asm(source_lines);
        write_binary(instructions, "test.bin");

    }
    catch(const ArgError& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "usage: " << argv[0] << " {path-to-source-assembly}" << std::endl;
    }
    catch(const AsmError& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "refer to the assmebly language specification for more information" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}