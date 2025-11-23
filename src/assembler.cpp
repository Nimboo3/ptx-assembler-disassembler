#include "instruction.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <cctype>

using namespace miniptx;

static std::string trim(const std::string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static std::vector<std::string> split_args(const std::string &s) {
    std::vector<std::string> out;
    std::string cur;
    for (size_t i=0;i<s.size();++i) {
        char c = s[i];
        if (c == ',' ) {
            if (!cur.empty()) {
                out.push_back(trim(cur));
                cur.clear();
            }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) out.push_back(trim(cur));
    return out;
}

// encode helpers
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

Decoded decode(Encoded enc) {
    Decoded d{};
    d.opcode = (enc >> 24) & 0xFF;
    d.rd = (enc >> 19) & 0x1F;
    d.rs1 = (enc >> 14) & 0x1F;
    d.rs2 = (enc >> 9) & 0x1F;
    // imm: low 14 or 24 bits, we keep sign when needed in disassembler
    d.imm = (int32_t)(enc & 0xFFFFFF); // callers will sign-extend if needed
    return d;
}

static bool parse_int(const std::string &s, int &out) {
    // simple decimal (allow negative)
    try {
        size_t idx;
        long v = std::stol(s, &idx, 0); // supports 0x hex
        if (idx != s.size()) return false;
        out = (int)v;
        return true;
    } catch (...) {
        return false;
    }
}

bool assemble_file(const std::string &inpath, const std::string &outpath, std::string &err) {
    std::ifstream ifs(inpath);
    if (!ifs) { err = "cannot open input"; return false; }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ifs, line)) lines.push_back(line);

    // first pass: collect labels -> instruction index
    std::unordered_map<std::string,int> labels;
    int pc = 0;
    for (size_t i=0;i<lines.size();++i) {
        std::string s = trim(lines[i]);
        if (s.empty() || s[0] == '#') continue;
        // label?
        if (s.back() == ':') {
            std::string name = trim(s.substr(0, s.size()-1));
            labels[name] = pc;
            continue;
        }
        pc++;
    }

    // second pass: assemble
    std::vector<Encoded> out;
    pc = 0;
    for (size_t i=0;i<lines.size();++i) {
        std::string raw = trim(lines[i]);
        if (raw.empty() || raw[0] == '#') continue;
        if (raw.back() == ':') continue; // label line

        // tokenise opcode and args
        std::istringstream iss(raw);
        std::string op;
        iss >> op;
        // uppercase op
        for (auto &c : op) c = std::toupper((unsigned char)c);
        auto opt = str_to_opcode(op);
        if (!opt) { err = "unknown opcode: " + op + " on line " + std::to_string(i+1); return false; }
        uint8_t opcode = *opt;
        std::string rest;
        std::getline(iss, rest);
        rest = trim(rest);
        auto args = split_args(rest);

        if (opcode == OP_MOV) {
            if (args.size() != 2) { err = "MOV needs 2 args"; return false; }
            auto rd = regname_to_id(args[0]); auto rs = regname_to_id(args[1]);
            if (!rd || !rs) { err="invalid reg in MOV"; return false; }
            out.push_back(encode_r(opcode, *rd, *rs, 0));
        } else if (opcode == OP_ADD || opcode == OP_MUL) {
            if (args.size() != 3) { err = "ADD/MUL needs 3 args"; return false; }
            auto rd = regname_to_id(args[0]); auto r1 = regname_to_id(args[1]);
            if (!rd || !r1) { err = "invalid reg"; return false; }
            // third arg can be reg or immediate -> if immediate, convert to ADDI
            int imm=0;
            if (parse_int(args[2], imm)) {
                // use ADDI encoding if within 14-bit signed
                if (imm < -8192 || imm > 8191) { err="imm out of range"; return false; }
                out.push_back(encode_i(OP_ADDI, *rd, *r1, imm & 0x3FFF));
            } else {
                auto r2 = regname_to_id(args[2]);
                if (!r2) { err="invalid reg or imm"; return false; }
                out.push_back(encode_r(opcode, *rd, *r1, *r2));
            }
        } else if (opcode == OP_ADDI) {
            if (args.size() != 3) { err = "ADDI needs 3 args"; return false; }
            auto rd = regname_to_id(args[0]); auto r1 = regname_to_id(args[1]);
            int imm=0;
            if (!parse_int(args[2], imm)) { err = "invalid imm"; return false; }
            if (imm < -8192 || imm > 8191) { err="imm out of range -8192..8191"; return false; }
            out.push_back(encode_i(opcode, *rd, *r1, imm & 0x3FFF));
        } else if (opcode == OP_LD) {
            if (args.size() != 2) { err="LD needs 2 args"; return false; }
            auto rd = regname_to_id(args[0]);
            int imm=0;
            if (!parse_int(args[1], imm)) { err="LD addr must be immediate"; return false; }
            if (imm < -8192 || imm > 8191) { err="LD addr out of range"; return false; }
            out.push_back(encode_i(opcode, *rd, 0, imm & 0x3FFF));
        } else if (opcode == OP_ST) {
            if (args.size() != 2) { err="ST needs 2 args"; return false; }
            auto rs = regname_to_id(args[0]);
            int imm=0;
            if (!parse_int(args[1], imm)) { err="ST addr must be immediate"; return false; }
            if (imm < -8192 || imm > 8191) { err="ST addr out of range"; return false; }
            out.push_back(encode_i(opcode, *rs, 0, imm & 0x3FFF));
        } else if (opcode == OP_BRA) {
            if (args.size() != 1) { err="BRA needs label"; return false; }
            std::string label = args[0];
            // label must exist
            auto it = labels.find(label);
            if (it == labels.end()) { err="unknown label: " + label; return false; }
            int target_pc = it->second;
            int rel = target_pc - pc - 1; // relative offset in instructions (branch PC-rel)
            // check 24-bit signed
            if (rel < -(1<<23) || rel > ((1<<23)-1)) { err="branch offset out of range"; return false; }
            out.push_back(encode_j(opcode, rel));
        } else if (opcode == OP_EXIT) {
            out.push_back(encode_r(opcode, 0,0,0));
        } else {
            err = "unsupported opcode in assembler";
            return false;
        }

        pc++;
    }

    // write binary file (little endian)
    std::ofstream ofs(outpath, std::ios::binary);
    if (!ofs) { err="cannot open output file"; return false; }
    for (auto e : out) {
        uint32_t v = e;
        ofs.write(reinterpret_cast<const char*>(&v), sizeof(v));
    }
    return true;
}
