#!/usr/bin/env bash
set -e
if [ $# -ne 3 ]; then
  echo "Usage: run_roundtrip.sh <asm_input.s> <asm_exec> <disasm_exec>"
  echo "Example: ./run_roundtrip.sh ../examples/test.s ./asm ./disasm"
  exit 2
fi

ASM_IN=$1
ASM_EXE=$2
DISASM_EXE=$3

TMPBIN=__tmp_prog.bin
TMPDIS=__tmp_prog.disasm

$ASM_EXE "$ASM_IN" -o "$TMPBIN"
$DISASM_EXE "$TMPBIN" -o "$TMPDIS"

echo "Disassembly output:"
cat "$TMPDIS"
echo
echo "Round-trip finished. Binary written to $TMPBIN, disasm to $TMPDIS"
