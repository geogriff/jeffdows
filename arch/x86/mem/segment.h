#ifndef _SEGMENT_H
#define _SEGMENT_H

// boot-time GDT indices
#define BOOT_NULL_INDEX 0
#define BOOT_CS_INDEX 1
#define BOOT_DS_INDEX (BOOT_CS_INDEX+1)

#define BOOT_CS (BOOT_CS_INDEX*8)
#define BOOT_DS (BOOT_DS_INDEX*8)

#endif /* _SEGMENT_H */
