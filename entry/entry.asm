BITS 16

extern __c_sectors
extern __stack_top

global _start

section .text
_start:
    ; Move MBR code to 0x600
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00
    mov es, ax
    mov ds, ax
    mov si, 0x7c00
    mov di, 0x0600
    mov cx, 0x200 ; 512 bytes
    cld
    rep
    movsb
    push ax
    push _loadc
    retf          ; JMP to _loadc
_loadc:
    ; NOTE: Current limit is 15 sectors. If the c binary is bigger, it will
    ; have to load itself.
    ; If __c_sectors > 15 then set to max of 15
    mov al, __c_sectors
    cmp al, 15
    jng _loadsectors
    mov al, 15
_loadsectors:
    mov ah, 2
    mov ch, 0  ; Cylinder
    mov cl, 2  ; # of sectors
    xor dx, dx
    mov bx, 0x800 ; 0x600 + 512 bytes (should be c entry point)

    int 0x13

    ; Set stack
    mov esp, __stack_top
    mov ebp, __stack_top

    ; Jump to c binary
    push 0x800
    retf