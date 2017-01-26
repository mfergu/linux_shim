
//http://bit.ly/2juhG81    
#define _GNU_SOURCE    

#include <stdio.h>    
#include <stdlib.h>    
#include <dlfcn.h>    
    
// intecepting malloc calls 

void* malloc(size_t size) {    
    
    void* (*libc_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");    
    
    void *mem = libc_malloc (size);    
    
    if(mem) {    
        fprintf (stderr, "[MALLOC] %zu bytes at %p", size, mem);    
    }    
    
    return mem;    
}    

