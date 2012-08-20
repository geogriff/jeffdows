.PHONY: all clean

all: all.o
clean:
	rm *.o

all.o: kprintf.o ctype.o stdlib.o
	$(LD) $(LDFLAGS) -o $@ $^
