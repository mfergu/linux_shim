#define _GNU_SOURCE    


#include <unistd.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <dlfcn.h>    
    

//linked list to record memory allocations
typedef struct alloc_t {
	size_t size;
	void* location;
	struct alloc_t* next;
	} alloc_t; 


void add_alloc(alloc_t* , size_t , unsigned long*);
void print_list(alloc_t*); 
void remove_alloc(alloc_t* , unsigned long* );


void __attribute__ ((constructor)) malloc_init(void);
void __attribute__ ((constructor)) free_init(void);
void __attribute__ ((destructor)) cleanup(void);
	

void* (*original_malloc)(size_t) = NULL;
void (*original_free)(void*) = NULL;


alloc_t my_allocs = {0,NULL,NULL};
size_t total_allocs = 0;
size_t total_space = 0;


//called when the library is unloaded
void cleanup(void) {
	//use this to function to close any files that were still open
	//or do any other clean up.
	print_list(&my_allocs);
}


void malloc_init(void) {
	if(original_malloc == NULL) {
		original_malloc = dlsym(RTLD_NEXT, "malloc");
	}
}


// intecepting malloc calls 
void* malloc(size_t size) {    
    
	//heap is a pointer to the return of the original malloc
	//it contains the location of the stored data
    void *heap = original_malloc(size);    
    
    if(heap) {    
		add_alloc(&my_allocs, size, heap);		
    }    
    
    return heap;    
}    


void free_init(void) {
	if(original_free == NULL) {
		original_free = dlsym(RTLD_NEXT, "free");
	}
}	


//intercepting free calls
void free(void* temp) {
	remove_alloc(&my_allocs, temp);
	original_free(temp);	
}


void add_alloc(alloc_t* head, size_t size, unsigned long* location) {

	alloc_t* current = head;

	while( current->next != NULL) {
		current = current->next;
	}

	alloc_t* temp = calloc(1, sizeof(alloc_t));	
	temp->size = size;
	temp->location = location;
	temp->next = NULL;
	current->next = temp;
	
}

void remove_alloc(alloc_t* head, unsigned long* remove_me) {
	
	if(head == NULL) {
		return;
	}
	alloc_t* current = head->next;
	alloc_t* previous = head;

	while( current->location != remove_me && current->next != NULL) {
		
		previous = previous->next;
		current = current->next;
		
	}
	if(current->next == NULL && current->location != remove_me) 
		return;
	
	alloc_t* temp = current;
	previous->next = current->next;
	
}

void print_list(alloc_t* head) {
	size_t total_leaks = 0;
	if(head == NULL) {
		return;
	}
	size_t num_of_leaks = 0, total_leak = 0;
	alloc_t* temp = head;
	while( temp != NULL) {
		if(temp->size > 0) {
			fprintf(stderr, "LEAK\t%zu\n", temp->size);
			++num_of_leaks;
			total_leak += temp->size;
		}
			temp = temp->next;
	}	

	fprintf(stderr, "TOTAL\t%zu\t%zu\n", num_of_leaks, total_leak );

}

