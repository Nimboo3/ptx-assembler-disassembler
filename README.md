# Mini PTX Assembler

A simple assembler and disassembler for a custom instruction set. This project lets you convert assembly code to binary and back again.

## What It Does

- **Assembler**: Converts human-readable assembly code (`.s` files) into binary machine code (`.bin` files)
- **Disassembler**: Converts binary machine code back into assembly code
- Supports basic operations like move, add, multiply, load, store, and branch instructions
- Works with labels for easier programming

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

Convert assembly to binary:

```bash
./asm input.s -o output.bin
```

Convert binary back to assembly:

```bash
./disasm input.bin -o output.s
```

## Example

Check `examples/test.s` for a sample assembly program.
