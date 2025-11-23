#include "instruction.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <map>

namespace miniptx {

static std::map<std::string, Opcode> opcode_map = {
    {"NOP", OP_NOP}, {"MOV", OP_MOV}, {"ADD", OP_ADD}, {"ADDI", OP_ADDI},
    {"MUL", OP_MUL}, {"LD", OP_LD}, {"ST", OP_ST}, {"BRA", OP_BRA},
    {"EXIT", OP_EXIT}
};

std::string opcode_to_str(uint8_t op) {
    for (auto &p : opcode_map) {
        if (p.second == op) return p.first;
    }
    return "UNK";
}

std::optional<uint8_t> str_to_opcode(const std::string &s) {
    auto it = opcode_map.find(s);
    if (it == opcode_map.end()) return std::nullopt;
    return static_cast<uint8_t>(it->second);
}

std::optional<uint8_t> regname_to_id(const std::string &s) {
    // expect r0..r31
    if (s.size() < 2 || (s[0] != 'r' && s[0] != 'R')) return std::nullopt;
    std::string num = s.substr(1);
    for (char c : num) if (!isdigit((unsigned char)c)) return std::nullopt;
    int v = std::stoi(num);
    if (v < 0 || v > 31) return std::nullopt;
    return static_cast<uint8_t>(v);
}

std::string id_to_regname(uint8_t id) {
    std::ostringstream ss;
    ss << 'r' << (int)id;
    return ss.str();
}

} // namespace miniptx
