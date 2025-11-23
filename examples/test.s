# Simple test program (labels allowed)
start:
    MOV r1, r0
    ADD r2, r1, r1
    ADD r3, r2, 5
    MUL r4, r3, r2
    LD r5, 128
    ST r4, 256
    BRA start
    EXIT
