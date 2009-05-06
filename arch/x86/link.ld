OUTPUT_FORMAT("elf32-i386")
ENTRY(_start)

__load_addr = 0x00100000;
__virt = 0xC0000000;

SECTIONS
{
  . = __virt + __load_addr;
  .text : AT(ADDR(.text) - __virt)
  {
    *(.text)
    *(.rodata)
    _etext = .;
  }

  .data : AT(ADDR(.data) - __virt)
  {
    *(.data)
    _edata = .;
  }

  .bss : AT(ADDR(.bss) - __virt)
  {
    *(.bss)
    *(COMMON)
    _end = .;
  }
}