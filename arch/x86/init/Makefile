.PHONY: all clean

all: all.o
clean:
	rm *.o

all.o: start.o init.o interrupt.o
	$(LD) $(LDFLAGS) -o $@ $^
