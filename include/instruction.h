#pragma once
#include <cstdint>
#include <string>
#include <optional>
#include <vector>

namespace miniptx {

// opcode enum
enum Opcode : uint8_t {
    OP_NOP  = 0,
    OP_MOV  = 1,
    OP_ADD  = 2,
    OP_ADDI = 3,
    OP_MUL  = 4,
    OP_LD   = 5,
    OP_ST   = 6,
    OP_BRA  = 7,
    OP_EXIT = 8
};

// simple 32-bit encoded instruction
using Encoded = uint32_t;

// helpers to encode/decode instruction formats
Encoded encode_r(uint8_t opcode, uint8_t rd, uint8_t rs1, uint8_t rs2);
Encoded encode_i(uint8_t opcode, uint8_t rd_rs, uint8_t rs1, int32_t imm); // imm is 14-bit signed
Encoded encode_j(uint8_t opcode, int32_t offset); // offset is 24-bit signed

struct Decoded {
    uint8_t opcode;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    int32_t imm; // used for I and J type (for J stored in imm)
};

Decoded decode(Encoded enc);

// text helpers
std::optional<uint8_t> regname_to_id(const std::string &s);
std::string id_to_regname(uint8_t id);
std::string opcode_to_str(uint8_t op);
std::optional<uint8_t> str_to_opcode(const std::string &s);

} // namespace miniptx
