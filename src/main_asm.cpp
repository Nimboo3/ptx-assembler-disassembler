#include <iostream>
#include <string>
#include <vector>
#include <cstring>

bool assemble_file(const std::string &inpath, const std::string &outpath, std::string &err);

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: asm input.s -o out.bin\n";
        return 1;
    }
    std::string in = argv[1];
    std::string out = "out.bin";
    for (int i=2;i<argc;i++) {
        if (std::string(argv[i]) == "-o" && i+1<argc) {
            out = argv[++i];
        }
    }
    std::string err;
    if (!assemble_file(in, out, err)) {
        std::cerr << "Assemble failed: " << err << "\n";
        return 2;
    }
    std::cout << "Wrote " << out << "\n";
    return 0;
}
