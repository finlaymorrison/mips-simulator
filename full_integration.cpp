#include "assembly.h"
#include "exceptions.h"
#include "mips.h"

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
        std::pair<std::string, std::string> files = validate_args(argc, argv);
        std::vector<std::string> source_lines = read_file(files.first);
        std::vector<Bits<32>> instructions = parse_asm(source_lines);
        MIPS mips(1000, 1000);
        mips.initialize_instruction_mem(instructions);
        mips.run();
        std::vector<std::string> mem_dump = mips.memory_dump();
        write_binary(mem_dump, argv[2]);

    }
    catch(const ArgError& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "usage: " << argv[0] << " {path-to-source-assembly} {path-to-output-binary}" << std::endl;
        return 3;
    }
    catch(const AsmError& e)
    {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "refer to the assmebly language specification for more information" << std::endl;
        return 2;
    }
    catch(const std::exception& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}