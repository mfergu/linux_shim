CC=clang
CFLAGS=-Weverything -g

BINS=leakcount.so leakcount

all: $(BINS)

leakcount: lc_run.c
	$(CC) $(CFLAGS) -o leakcount lc_run.c 

leakcount.so:  leakcount.c    
	$(CC) $(CFLAGS) -fPIC -shared -o leakcount.so leakcount.c -ldl

clean:
	rm $(BINS)

