all: kernel-x86

CFLAGS += -m32
LDFLAGS += -melf_i386

core-o += arch/x86/core/

src-dirs += arch/x86/boot/

kernel-x86: arch/x86/link.ld arch/x86/boot/kernel-boot.o kernel.o
	$(LD) $(LDFLAGS) -o $@ $+

arch/x86/boot/kernel-boot.o: arch/x86/boot/ ;
