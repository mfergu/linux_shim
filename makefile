#not completed
CC=clang
CFLAGS=-Weverything -g -std=c11

c   = leakcount.c    
h   = leakcount.h   
m   = main.c    
t   = leakcount    
    
leakcount    = main.o leakcount.o    
    
all:        $t    
clean:              ; rm -f $t *.o    
    
test::      leakcount        ; leakcount     
    
leakcount:       $(leakcount)     ; $(CC) $(CFLAGS) -o $@ $(leakcount)    
    

