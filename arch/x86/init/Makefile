.PHONY: all clean

all: all.o
clean:
	rm *.o

all.o: start.o init.o interrupt.o
	$(LD) $(LDFLAGS) -r -o $@ $+

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $+