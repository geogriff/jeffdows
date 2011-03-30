.PHONY: all clean

all: all.o
clean:
	rm *.o

all.o: main.o init.o mem.o pmem.o
	$(LD) $(LDFLAGS) -o $@ $^
