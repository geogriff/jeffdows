.PHONY: all clean

all: all.o
clean:
	rm *.o

all.o: printf.o trap.o trap_entry.o
	$(LD) $(LDFLAGS) -r -o $@ $+

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $+
