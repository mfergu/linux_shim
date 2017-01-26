CC=clang
CFLAGS=-Weverything -g

all: $(BINS)

leakcount.so:  leakcount.c    
    $(CC) $(CFLAGS) -fPIC -shared -o shim.so shim.c -ldl

clean:
	rm $(BINS)

