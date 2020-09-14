#pragma once
#include "type.h"

// vm architecture

// option 1
    // separate instructions by constant/register
    // limtied to max 16 registers
    // e.g. MOVR, MOVC

// -------------- I think i'll go with this option -------------------------
// option 2
    // separate instruction LOADC, which loads constant into register
    // limtied to max 15 registers now
    // e.g. LOADC 0xF;
    //      MOV A, __CONST;

struct VMContext;

typedef void(*VM_CFUNCTION)(struct VMContext* ctx);

// 16 Registers
// cmp flag
// Independent RAM of 512 bytes

// cmp flags (1 = true, 0 = false)
// 2 bit - Greater than
// 1 bit - Less than
// 0 bit - Equal to
typedef struct VMContext {
    uint8_t cmp_flags;

    uint8_t ram[512];
    uint16_t registers[16];
    VM_CFUNCTION* cfunctions;

    uint16_t pc;
    uint16_t start;
    uint16_t end;
} VMContext;

#define VM_GETR(x) ctx->registers[x]
#define VM_SETR(x, v) ctx->registers[x] = v;

#define RR_SRC instruction->r & 0xF
#define RR_DST instruction->r >> 4

// R = Register
// C = Const
// R(src) = LO
// R(dst) = HI
// e.g 00010011 = 0x13 = mov r1, r3
typedef struct OP_RR {
    uint8_t op;
    uint8_t r;
} OP_RR;

typedef OP_RR OP_R;

typedef OP_RR OP_C_BYTE;

typedef struct OP_RC_WORD {
    uint8_t op;
    uint8_t r_dst;
    uint16_t c;
} OP_RC_WORD;

typedef struct OP_RC_BYTE {
    uint8_t op;
    uint8_t r_dst;
    uint8_t c;
} OP_RC_BYTE;

void vm_execute(VMContext* ctx);