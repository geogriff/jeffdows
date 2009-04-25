.PHONY: all clean

all: kernel-boot.o
clean:
	rm *.o

kernel-boot.o: boot.o init.o
	$(LD) $(LDFLAGS) -r -o $@ $+

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $+