#pragma once

// Notation
// rN = nth register

// operations with two registers
// OP (8 bit) -> IN (8 bit)[LO bit (r0), HI (r1)]

// operation one register
// OP (8 bit) -> IN (8 bit)[r0]

// operation one register, one constant
// OP (8 bit) -> IN (8 bit)[r0], CONST (16 bit)[c]

// naming: VM_[OP][C/R]
typedef enum {
    VM_CMP=0,
    VM_MOV=1,
    VM_JMPCND=2, // jmp r0, r1-as-cmpflag
    VM_JMP=3, // jmp r0
    VM_LOADC=4, // mov __const/r0, constantval
    VM_CALLC=5, // call [byte]

    // bitwise: dst = dst (operator) src
    VM_AND=6,
    VM_OR=7,
    VM_XOR=8,
    VM_NOT=9, // dst = ~dst
    VM_SHR=10,
    VM_SHL=11,

    // arithmetic: dst = dst (operator) src
    VM_ADD=12,
    VM_SUB=13,
    VM_MUL=14,
    VM_DIV=15,
} OpCode;