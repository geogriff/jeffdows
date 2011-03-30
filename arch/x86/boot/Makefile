.PHONY: all clean

all: kernel-boot.o
clean:
	rm *.o

kernel-boot.o: boot.o
	$(LD) $(LDFLAGS) -o $@ $+
