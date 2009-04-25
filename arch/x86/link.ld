OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)
phys = 0x00100000;

SECTIONS
{
  .text phys : AT(phys)
  {
    code = .;
    *(.text)
    *(.rodata)
    _etext = .;
    __etext = .;
    . = ALIGN(4096);
  }
  .data : AT (phys + (data - code))
  {
    data = .;
    *(.data)
    _edata = .;
    __edata = .;
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    *(COMMON)
    . = ALIGN(4096);
    _end = .;
    __end = .;
  }
}