.PHONY: all clean

all: all.o
clean:
	rm *.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

all.o: printf.o
	$(LD) $(LDFLAGS) -r -o $@ $+