#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#define MULTIBOOT_EAX_MAGIC 0x2BADB002

typedef struct module_info {
  unsigned long mod_start;
  unsigned long mod_end;
  unsigned long string;
  unsigned long reserved;
} module_info_t;

typedef struct aout_symbol_table {
  unsigned long tabsize;
  unsigned long strsize;
  unsigned long addr;
  unsigned long reserved;
} aout_symbol_table_t;

typedef struct elf_section_header_table {
  unsigned long num;
  unsigned long size;
  unsigned long addr;
  unsigned long shndx;
} elf_section_header_table_t;

typedef struct mmap_info {
  unsigned long size;
  unsigned long base_addr_low;
  unsigned long base_addr_high;
  unsigned long length_low;
  unsigned long length_high;
  unsigned long type;
} mmap_info_t;

typedef struct drive_info {
  unsigned long size;
  unsigned long drive_number;
  unsigned long drive_mode;
  unsigned long drive_cylinders;
  unsigned long drive_heads;
  unsigned long drive_sectors;
  unsigned short drive_ports;
} drive_info_t;

typedef struct apm_table {
  unsigned short version;
  unsigned short cseg;
  unsigned long offset;
  unsigned short cseg_16;
  unsigned short dseg;
  unsigned short flags;
  unsigned short cseg_len;
  unsigned short cseg_16_len;
  unsigned short dseg_len;
} apm_table_t;

typedef struct multiboot_info {
  unsigned long flags;
  unsigned long mem_lower;
  unsigned long mem_upper;
  unsigned long boot_device;
  unsigned long cmdline;
  unsigned long mods_count;
  unsigned long mods_addr;
  union {
    aout_symbol_table_t aout_symbol_table;
    elf_section_header_table_t elf_section_header_table;
  };
  unsigned long mmap_length;
  unsigned long mmap_addr;
  unsigned long drives_length;
  unsigned long config_table;
  unsigned long boot_loader_name;
  unsigned long apm_table;
  unsigned long vbe_control_info;
  unsigned long vbe_mode_info;
  unsigned long vbe_mode;
  unsigned long vbe_interface_seg;
  unsigned long vbe_interface_off;
  unsigned long vbe_interface_len;
} multiboot_info_t;  

#endif /* _MULTIBOOT_H */
