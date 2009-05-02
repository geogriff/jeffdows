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
  }
  .data : AT (phys + (_data - _text))
  {
    _data = .;
    *(.data)
    _edata = .;
    __edata = .;
  }
  .bss : AT(phys + (_bss - _text))
  {
    _bss = .;
    *(.bss)
    *(COMMON)
    _end = .;
    __end = .;
  }
}