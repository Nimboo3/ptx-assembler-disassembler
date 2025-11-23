#include "instruction.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <sstream>

using namespace miniptx;

Encoded encode_r(uint8_t opcode, uint8_t rd, uint8_t rs1, uint8_t rs2) {
    Encoded e = 0;
    e |= (uint32_t)opcode << 24;
    e |= (uint32_t)(rd & 0x1F) << 19;
    e |= (uint32_t)(rs1 & 0x1F) << 14;
    e |= (uint32_t)(rs2 & 0x1F) << 9;
    return e;
}

Encoded encode_i(uint8_t opcode, uint8_t rd_rs, uint8_t rs1, int32_t imm) {
    Encoded e = 0;
    e |= (uint32_t)opcode << 24;
    e |= (uint32_t)(rd_rs & 0x1F) << 19;
    e |= (uint32_t)(rs1 & 0x1F) << 14;
    int32_t imm14 = imm & 0x3FFF;
    e |= (uint32_t)imm14;
    return e;
}

Encoded encode_j(uint8_t opcode, int32_t offset) {
    Encoded e = 0;
    e |= (uint32_t)opcode << 24;
    uint32_t off = (uint32_t)(offset & 0xFFFFFF);
    e |= off;
    return e;
}

static int32_t sign_extend(int32_t value, int bits) {
    int shift = 32 - bits;
    return (value << shift) >> shift;
}

std::string disasm_instruction(Encoded e, int pc) {
    Decoded d = decode(e);
    std::ostringstream ss;
    auto opstr = opcode_to_str(d.opcode);
    if (d.opcode == OP_MOV) {
        ss << opstr << " " << id_to_regname(d.rd) << ", " << id_to_regname(d.rs1);
    } else if (d.opcode == OP_ADD || d.opcode == OP_MUL) {
        ss << opstr << " " << id_to_regname(d.rd) << ", " << id_to_regname(d.rs1) << ", " << id_to_regname(d.rs2);
    } else if (d.opcode == OP_ADDI) {
        int imm = sign_extend((int)d.imm & 0x3FFF, 14);
        ss << opstr << " " << id_to_regname(d.rd) << ", " << id_to_regname(d.rs1) << ", " << imm;
    } else if (d.opcode == OP_LD) {
        int imm = sign_extend((int)d.imm & 0x3FFF, 14);
        ss << opstr << " " << id_to_regname(d.rd) << ", " << imm;
    } else if (d.opcode == OP_ST) {
        int imm = sign_extend((int)d.imm & 0x3FFF, 14);
        ss << opstr << " " << id_to_regname(d.rd) << ", " << imm;
    } else if (d.opcode == OP_BRA) {
        int off = sign_extend((int)d.imm & 0xFFFFFF, 24);
        int target = pc + 1 + off;
        ss << opstr << " " << target; // disassemble to numeric target for simplicity
    } else if (d.opcode == OP_EXIT) {
        ss << opstr;
    } else {
        ss << "UNK";
    }
    return ss.str();
}

bool disassemble_file(const std::string &inpath, const std::string &outpath, std::string &err) {
    std::ifstream ifs(inpath, std::ios::binary);
    if (!ifs) { err="cannot open binary"; return false; }
    std::vector<Encoded> insts;
    while (true) {
        uint32_t v;
        ifs.read(reinterpret_cast<char*>(&v), sizeof(v));
        if (!ifs) break;
        insts.push_back(v);
    }
    std::ofstream ofs(outpath);
    if (!ofs) { err="cannot open output"; return false; }
    for (size_t i=0;i<insts.size();++i) {
        ofs << disasm_instruction(insts[i], (int)i) << "\n";
    }
    return true;
}
