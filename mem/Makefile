.PHONY: all clean

all: all.o
clean:
	rm *.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

all.o: pmem.o
	$(LD) $(LDFLAGS) -r -o $@ $+