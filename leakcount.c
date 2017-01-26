
//http://bit.ly/2juhG81    
#define _GNU_SOURCE    

void __attribute__ ((constructor)) malloc_init(size_t);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>    
#include <stdlib.h>    
#include <dlfcn.h>    
    
void* (*original_malloc)(size_t) = NULL;

//called when the library is unloaded
void cleanup(void) {
	//use this to function to close any files that were still open
	//or do any other clean up.
	printf("cleaning up.\n");
}

void malloc_init(void) {
	if(original_malloc == NULL) {
		original_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	printf("initializing library.\n");
}
// intecepting malloc calls 
void* malloc(size_t size) {    
    
    void* (*libc_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");    
    
    void *mem = libc_malloc (size);    
    
    if(mem) {    
        fprintf (stderr, "[MALLOC] %zu bytes at %p", size, mem);    
    }    
    
    return mem;    
}    

