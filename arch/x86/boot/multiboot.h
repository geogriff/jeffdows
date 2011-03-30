#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#define MULTIBOOT_HEADER_ALIGN_PAGE 1<<0
#define MULTIBOOT_HEADER_MEM_INFO 1<<1
#define MULTIBOOT_HEADER_VIDEO_INFO 1<<2
#define MULTIBOOT_HEADER_INTERNAL_RELOC 1<<16

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_HEADER_FLAGS (MULTIBOOT_HEADER_ALIGN_PAGE | \
                                MULTIBOOT_HEADER_MEM_INFO | \
                                MULTIBOOT_HEADER_INTERNAL_RELOC)
#define MULTIBOOT_HEADER_CHECKSUM -(MULTIBOOT_HEADER_MAGIC + \
                                    MULTIBOOT_HEADER_FLAGS)


#define MULTIBOOT_MAGIC 0x2BADB002

#define MULTIBOOT_FLAG_MEM (1<<0)
#define MULTIBOOT_FLAG_BOOT_DEV (1<<1)
#define MULTIBOOT_FLAG_CMDLINE (1<<2)
#define MULTIBOOT_FLAG_MODS (1<<3)
#define MULTIBOOT_FLAG_AOUT_SYMS (1<<4)
#define MULTIBOOT_FLAG_ELF_SYMS (1<<5)
#define MULTIBOOT_FLAG_MMAP (1<<6)
#define MULTIBOOT_FLAG_DRIVES (1<<7)
#define MULTIBOOT_FLAG_CFG_TABLE (1<<8)
#define MULTIBOOT_FLAG_LDR_NAME (1<<9)
#define MULTIBOOT_FLAG_APM_TABLE (1<<10)
#define MULTIBOOT_FLAG_VBE_INFO (1<<11)

#ifndef __ASM__

#include <stdint.h>

typedef struct module_info {
  uint32_t mod_start;
  uint32_t mod_end;
  uint32_t string;
  uint32_t reserved;
} module_info_t;

typedef struct aout_symbol_table {
  uint32_t tabsize;
  uint32_t strsize;
  uint32_t addr;
  uint32_t reserved;
} aout_symbol_table_t;

typedef struct elf_section_header_table {
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
} elf_section_header_table_t;

typedef struct mmap_info {
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
} mmap_info_t;

typedef struct drive_info {
  uint32_t size;
  uint32_t drive_number;
  uint32_t drive_mode;
  uint32_t drive_cylinders;
  uint32_t drive_heads;
  uint32_t drive_sectors;
  uint16_t drive_ports;
} drive_info_t;

typedef struct apm_table {
  uint16_t version;
  uint16_t cseg;
  uint32_t offset;
  uint16_t cseg_16;
  uint16_t dseg;
  uint16_t flags;
  uint16_t cseg_len;
  uint16_t cseg_16_len;
  uint16_t dseg_len;
} apm_table_t;

typedef struct multiboot_info {
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  uint32_t cmdline;
  uint32_t mods_count;
  uint32_t mods_addr;
  union {
    aout_symbol_table_t aout_symbol_table;
    elf_section_header_table_t elf_section_header_table;
  } syms;
  uint32_t mmap_length;
  uint32_t mmap_addr;
  uint32_t drives_length;
  uint32_t config_table;
  uint32_t boot_loader_name;
  uint32_t apm_table;
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint32_t vbe_mode;
  uint32_t vbe_interface_seg;
  uint32_t vbe_interface_off;
  uint32_t vbe_interface_len;
} multiboot_info_t;

#endif /* __ASM__ */

#endif /* _MULTIBOOT_H */
