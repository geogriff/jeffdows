.PHONY: all clean

all: all.o
clean:
	rm *.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

all.o: main.o init.o mem.o pmem.o
	$(LD) $(LDFLAGS) -r -o $@ $+