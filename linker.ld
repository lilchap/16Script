ENTRY(_start)
SECTIONS
{
  . = 0x600;
  .text : {
    entry.o(.text)
    /* Define MBR Signature here */
    FILL(0x0)
    . = 512 - 2;
    SHORT(0xaa55);

    /* C Code*/
    __c_start = .; /* Should always be 0x800 */

    *(.text.main)
    *(.text)
    *(.data)
    *(.rdata)
    *(.rodata)
    *(.bss)
    *(COMMON)

    __c_end = .;
    /* Calculate how many sectors section takes up */
    __c_sectors = ((__c_end - __c_start) / 512) - 0x600 + 1;

    /* Align to next sector */
    . = ALIGN(512);

    /* Put script binary here. Remember to load sector again */
    __script_start = .;
    script.o(.script_bin)
    __script_end = .;

    /* + 1 not necessary */
    /* start loading at the next sector after __c_end/__script_start */
    __script_sectors = ((__script_end - __script_start) / 512) - 0x600 + 1;
  }
  /DISCARD/ : {
    *(.eh_frame)
  }
  __stack_bottom = .;
  . = . + 0x1000;
  __stack_top = .;
}
