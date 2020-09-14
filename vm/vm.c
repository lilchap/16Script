#include "vm.h"
#include "opcodes.h"

void vm_execute(VMContext* ctx) {
    ctx->pc = ctx->start;
    for (;;) {
        if (ctx->pc >= ctx->end)
            break;
        
        uint8_t* pc_op = ctx->pc;

        switch (*pc_op) {
            case VM_CMP:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                uint16_t dst_value = VM_GETR(RR_DST);
                uint16_t src_value = VM_GETR(RR_SRC);
                ctx->cmp_flags ^= ctx->cmp_flags; // clear cmp_flags
                ctx->cmp_flags |= (dst_value == src_value); // eq (0 bit)
                ctx->cmp_flags |= ((dst_value < src_value) << 1);// lt (1 bit)
                ctx->cmp_flags |= ((dst_value > src_value) << 2); // gt (2 bit)
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_MOV:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                uint16_t src_value = VM_GETR(RR_SRC);
                VM_SETR(RR_DST, src_value);
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_JMPCND:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                uint16_t dst_value = VM_GETR(RR_DST);
                uint16_t src_value = VM_GETR(RR_SRC);
                if (ctx->cmp_flags == (src_value & 7))
                    ctx->pc += (signed)dst_value + sizeof(OP_RR);
                else
                    ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_JMP:{
                OP_R* instruction = (OP_R*)(pc_op);
                ctx->pc += (signed)VM_GETR(instruction->r) + sizeof(OP_R);
                break;
            }
            case VM_LOADC:{
                OP_RC_WORD* instruction = (OP_RC_WORD*)(pc_op);
                VM_SETR(instruction->r_dst, instruction->c);
                ctx->pc += sizeof(OP_RC_WORD);
                break;
            }
            case VM_CALLC:{
                OP_C_BYTE* instruction = (OP_C_BYTE*)(pc_op);
                VM_CFUNCTION func = (VM_CFUNCTION)(ctx->cfunctions[instruction->r]);
                func(ctx);
                ctx->pc += sizeof(OP_C_BYTE);
                break;
            }
            // bitwise
            case VM_AND:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) & VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_OR:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) | VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_XOR:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) ^ VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_NOT:{
                OP_R* instruction = (OP_R*)(pc_op);
                VM_SETR(instruction->r, ~(instruction->r));
                ctx->pc += sizeof(OP_R);
                break;
            }
            case VM_SHR:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) >> VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_SHL:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) << VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            // arithmetic
            case VM_ADD:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) + VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_SUB:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) - VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_MUL:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) * VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            case VM_DIV:{
                OP_RR* instruction = (OP_RR*)(pc_op);
                VM_SETR(RR_DST, VM_GETR(RR_DST) / VM_GETR(RR_SRC));
                ctx->pc += sizeof(OP_RR);
                break;
            }
            // remember to put break; on normal cases! i keep forgetting
            default:{
                // panic
                __asm__ __volatile__ ("hlt;");
                break;
            }
        }
    }

    // dbg
    __asm__ __volatile__ (
        ".intel_syntax;"
        "xchg bx, bx;"
        "nop;"
        "push eax;"
        "mov ax, %0;"
        "pop eax;"
        ".att_syntax;"
        :
        : "r"(ctx->registers[1])//"r"(ctx->registers[1])
        : "eax"
    );
}