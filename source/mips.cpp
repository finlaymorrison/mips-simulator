#include "mips.h"

#include <array>

MIPS::MIPS(size_t instruction_capacity, size_t data_capacity) :
    fetch(instruction_capacity), mem_read(data_capacity), t_counter(0)
{
    clear(ifid_reg);
    clear(idex_reg);
    clear(exmem_reg);
    clear(memwb_reg);
    inc = true;
}

void MIPS::initialize_instruction_mem(const std::vector<Bits<32>>& instructions)
{
    fetch.write_instructions(instructions);
}

bool is_jmp_op(const Bits<6>& opcode, const Bits<6>& alu_opcode)
{
    int opcode_i = bin_to_int<6>(opcode);
    int alu_op_i = bin_to_int<6>(alu_opcode);
    return opcode_i == 4 || opcode_i == 5 || opcode_i == 6 || opcode_i == 10 || (opcode_i == 0 && alu_op_i == 8);
}

bool is_write_op(const Bits<6>& opcode)
{
    int opcode_i = bin_to_int<6>(opcode);
    return opcode_i == 0 || opcode_i == 1 || opcode_i == 2 || opcode_i ==3 || opcode_i == 7 || opcode_i == 9;
}

void MIPS::check_pipeline(InterstageReg& ifid_n, InterstageReg& idex_n, InterstageReg& exmem_n, InterstageReg& memwb_n)
{
    /* checking for flushes due to branches/jumps */
    /* detect in idex_n -- if flush -- do not update PC, ifid cleared */
    Bits<6> idex_opcode = extract_bits<32,6>(idex_n.instruction, 0);
    Bits<6> idex_alu_opcode =  extract_bits<32,6>(idex_n.instruction, 26);
    if (is_jmp_op(idex_opcode,idex_alu_opcode))
    {
        std::cout << "stall 1" << std::endl;
        clear(ifid_reg);
        idex_reg = idex_n;
        exmem_reg = exmem_n;
        memwb_reg = memwb_n;
        return;
    }
    /* detect in exmem_n -- if flush -- do not update PC, ifid cleared, idex cleared */
    Bits<6> exmem_opcode = extract_bits<32,6>(exmem_n.instruction, 0);
    Bits<6> exmem_alu_opcode =  extract_bits<32,6>(exmem_n.instruction, 26);
    if (is_jmp_op(exmem_opcode,exmem_alu_opcode))
    {
        std::cout << "stall 2" << std::endl;
        clear(ifid_reg);
        clear(idex_reg);
        exmem_reg = exmem_n;
        memwb_reg = memwb_n;
        return;
    }
    /* detect in memwb_n -- if flush -- do not update PC, ifid cleared, idex cleared, exmem cleared */
    Bits<6> memwb_opcode = extract_bits<32,6>(memwb_n.instruction, 0);
    Bits<6> memwb_alu_opcode =  extract_bits<32,6>(memwb_n.instruction, 26);
    if (is_jmp_op(memwb_opcode,memwb_alu_opcode))
    {
        std::cout << "stall 3" << std::endl;
        clear(ifid_reg);
        clear(idex_reg);
        clear(exmem_reg);
        memwb_reg = memwb_n;
        return;
    }
    /* detect in memwb_reg -- if flush -- do not update PC, ifid cleared, idex cleared, exmem cleared */
    Bits<6> memwb_opcode_r = extract_bits<32,6>(memwb_reg.instruction, 0);
    Bits<6> memwb_alu_opcode_r =  extract_bits<32,6>(memwb_reg.instruction, 26);
    if (is_jmp_op(memwb_opcode_r,memwb_alu_opcode_r))
    {
        std::cout << "stall 4" << std::endl;
        clear(ifid_reg);
        clear(idex_reg);
        clear(exmem_reg);
        clear(memwb_reg);
        return;
    }

    /* checking for stalls due to source values not ready */
    /* detect in idex_n -- if stall -- do not update PC, ifid stays constant, idex is cleared */
    /* register a checking */
    Bits<5> reg_b = extract_bits<32,5>(ifid_reg.instruction, 11);
    Bits<5> reg_c = extract_bits<32,5>(ifid_reg.instruction, 16);
    int reg_b_i = bin_to_int<5>(reg_b);
    int reg_c_i = bin_to_int<5>(reg_c);
    if (is_write_op(extract_bits<32,6>(idex_reg.instruction, 0)))
    {
        Bits<5> dest_reg = extract_bits<32,5>(idex_reg.instruction, 6);
        int dest_reg_i = bin_to_int<5>(dest_reg);
        if (reg_b_i && (dest_reg_i == reg_b_i) || reg_c_i && (dest_reg_i == reg_c_i))
        {
            std::cout << "stall 5" << std::endl;
            clear(idex_reg);
            exmem_reg = exmem_n;
            memwb_reg = memwb_n;
            return;
        }
    } 
    if (is_write_op(extract_bits<32,6>(exmem_reg.instruction, 0)))
    {
        Bits<5> dest_reg = extract_bits<32,5>(exmem_reg.instruction, 6);
        int dest_reg_i = bin_to_int<5>(dest_reg);
        if (reg_b_i && (dest_reg_i == reg_b_i) || reg_c_i && (dest_reg_i == reg_c_i))
        {
            std::cout << "stall 6" << std::endl;
            clear(idex_reg);
            memwb_reg = memwb_n;
            exmem_reg = exmem_n;
            memwb_reg = memwb_n;
            return;
        }
    }
    if (is_write_op(extract_bits<32,6>(memwb_reg.instruction, 0)))
    {
        Bits<5> dest_reg = extract_bits<32,5>(memwb_reg.instruction, 6);
        int dest_reg_i = bin_to_int<5>(dest_reg);
        if (reg_b_i && (dest_reg_i == reg_b_i) || reg_c_i && (dest_reg_i == reg_c_i))
        {
            std::cout << "stall 7" << std::endl;
            clear(idex_reg);
            exmem_reg = exmem_n;
            memwb_reg = memwb_n;
            return;
        }
    }
    ifid_reg = ifid_n;
    idex_reg = idex_n;
    exmem_reg = exmem_n;
    memwb_reg = memwb_n;
    fetch.inc_pc();
    
}

int MIPS::clock_cycle()
{
    std::cout << "fetch" << std::endl;
    InterstageReg ifid_n = fetch.run();
    
    std::cout << "decode" << std::endl;
    InterstageReg idex_n = decode.run(ifid_reg);
    
    std::cout << "execute" << std::endl;
    InterstageReg exmem_n = execute.run(idex_reg);
    
    std::cout << "memread" << std::endl;
    InterstageReg memwb_n = mem_read.run(exmem_reg, fetch);
    
    std::cout << "writeback" << std::endl;
    writeback.run(memwb_reg, decode);

    check_pipeline(ifid_n, idex_n, exmem_n, memwb_n);

    decode.print_registers();

    Bits<6> opcode = extract_bits<32,6>(exmem_reg.instruction, 0);
    if (bin_to_int<6>(opcode) == 11)
    {
        /* return 1 to exit the loop in calling function */
        return 1;
    }
    return 0;
}

void MIPS::run_step()
{
    for (;;)
    {
        std::cout << "press enter to step forward... ";
        std::cin.ignore();
        if(clock_cycle()) break;
    }
}

void MIPS::run()
{
    for (;;)
    {
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
        if(clock_cycle()) break;
    }
}

std::vector<std::string> MIPS::memory_dump() const
{
    return mem_read.memory_dump();
}