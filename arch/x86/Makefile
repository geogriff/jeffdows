.PHONY: all
all: kernel

CFLAGS += -m32
ASFLAGS += -m32
LDFLAGS += -melf_i386

init-o += arch/x86/init/
core-o += arch/x86/core/

src-dirs += arch/x86/boot/

kernel: arch/x86/link.ld arch/x86/boot/kernel-boot.o kernel.o
	$(LD) $(LDFLAGS) -T arch/x86/link.ld -o $@ $^

arch/x86/boot/kernel-boot.o: arch/x86/boot/ ;
