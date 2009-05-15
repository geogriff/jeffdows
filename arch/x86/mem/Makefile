.PHONY: all clean

all: 
clean:
	rm *.o

all.o: 
	$(LD) $(LDFLAGS) -r -o $@ $+

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $+

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $+
