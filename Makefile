SRCDIR = $(CURDIR)

# get the architecure from the shell (if not specified)
ARCH ?= $(shell uname -m | sed -e s/i.86/x86/)

CC ?= gcc
LD ?= ld

CFLAGS += -ggdb -nostdlib -nostdinc -I$(SRCDIR) -I$(SRCDIR)/arch/$(ARCH)
LDFLAGS += -g

export CC LD
export CFLAGS LDFLAGS

# this will get some architecture specific vars/targets for us
include arch/$(ARCH)/Makefile

# add machine indepentdent dirs
init-o += init/

# src directories
kernel-dirs := $(filter %/, $(init-o) $(core-o))
src-dirs += $(kernel-dirs)

# references to dirs will really need their all.o files
init-o := $(patsubst %/, %/all.o, $(init-o))
core-o := $(patsubst %/, %/all.o, $(core-o))

# descend into directories
.PHONY: $(src-dirs)
$(src-dirs):
	$(MAKE) -C $@

# kernel objects
kernel-init := $(init-o)
kernel-main := $(core-o)

# link the kernel.o object
kernel.o: $(kernel-init) $(kernel-main)
	$(LD) $(LDFLAGS) -r -o $@ $^

# to build these objects, we need to descend into their directories
$(kernel-init) $(kernel-main): $(kernel-dirs) ;

.PHONY: clean
clean:
	find . -name '*.o' -type f -exec rm -f \{\} \;