.PHONY: all clean

all: all.o
clean:
	rm *.o

all.o: pmem.o kmalloc.o
	$(LD) $(LDFLAGS) -o $@ $^
