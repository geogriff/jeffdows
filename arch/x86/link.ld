OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)
phys = 0x00100000;
virt = 0xC0100000;

SECTIONS
{
  .text virt : AT(phys)
  {
    _begin = .;
    _text = .;
    *(.text)
    *(.rodata)
    _etext = .;
    __etext = .;
    . = ALIGN(4096);
  }
  .data : AT (phys + (_data - _text))
  {
    _data = .;
    *(.data)
    _edata = .;
    __edata = .;
    . = ALIGN(4096);
  }
  .bss : AT(phys + (_bss - _text))
  {
    _bss = .;
    *(.bss)
    *(COMMON)
    . = ALIGN(4096);
    _end = .;
    __end = .;
  }
}