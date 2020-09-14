//#include "vm.h"

//static VMContext ctx;
#include "memloc.h"
#include "vm.h"

#define dbg_print(x) \
    __asm__ __volatile__ ( \
        ".intel_syntax;" \
        "push ax;"                   \
        "mov ax, %0;" \
        "xchg bx, bx;" \
        "pop ax;" \
        ".att_syntax;" \
        : \
        : "r"(x) \
    );

void absolute_disk_read(uint16_t lba_start, uint16_t buffer, uint8_t num_sectors) {
    const uint8_t read_c = LBA_GET_C(lba_start);
    const uint8_t read_h = LBA_GET_H(lba_start);
    const uint8_t read_s = LBA_GET_S(lba_start);

    // high 16 - (LO [16-23] read_h)
    // low 16 - (HI [8-15]read_c, LO [0-7] read_s)
    uint32_t read_chs = (num_sectors << 24) | (read_h << 16) | (read_c << 8) | read_s;

    // reason for storing chs in 32-bit register is
    // because gcc will run out of registers to use otherwise
    __asm__ __volatile__ (
        ".intel_syntax;"
        //"xchg bx, bx;"
        "mov ecx, %0;"
        "mov edx, %0;"
        "shr edx, 8;"
        "mov dl, 0;"
        "mov bx, %1;"
        "mov eax, %0;"
        "shr eax, 24;"
        "mov ah, 2;"
        "int 0x13;"
        ".att_syntax;"
        :
        : "r"(read_chs), "r"(buffer)
        : "eax", "edx", "ecx", "ebx"
    );
}

VM_CFUNCTION embedded_cfunction1(VMContext* ctx) {
    uint16_t r3 = ctx->registers[3];
    if (r3 == 0x1337) {
        ctx->registers[3] = 0x666;
    }
}

static VMContext ctx;

static char _testcode[] = {
    0x04, 0x03, 0x37, 0x13, // loadc 3 1337
    0x05, 0x00, // callc 0
    0x01, 0x13, // mov 1, 3
};

static VM_CFUNCTION cfunctionlist[] = {
    embedded_cfunction1
};

C_ENTRY uint16_t __main() {
    // step 1: load script binary
    const uint16_t c_end_sector = (READSYM(_c_end) - 0x600) / 0x200; // lba format
    uint8_t script_sectors = (uint8_t)READSYM(_script_sectors);

    if (script_sectors > 15)
        script_sectors = 15;

    // lba address to start reading from
    uint16_t lba_start = c_end_sector + 1; //+1 = next sector
    absolute_disk_read(lba_start, READSYM(_c_end), script_sectors);

    //ctx.start = READSYM(_c_end);
    //ctx.end = (READSYM(_script_end) - READSYM(_script_start)) + ctx.start;
    ctx.start = _testcode;
    ctx.end = sizeof(_testcode) + _testcode;
    ctx.pc = ctx.start;
    ctx.cfunctions = cfunctionlist;
    vm_execute(&ctx);
}