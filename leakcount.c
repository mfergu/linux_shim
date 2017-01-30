//http://bit.ly/2kqkn93
//http://bit.ly/2juhG81    
#define _GNU_SOURCE    

void __attribute__ ((constructor)) malloc_init(void);
void __attribute__ ((constructor)) free_init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <unistd.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <dlfcn.h>    
    
//linked list to record memory allocations
typedef struct alloc_t {
	unsigned int size;
	long * location;
	struct alloc_t* next;
	} alloc_t; 

void add_alloc(alloc_t* head, unsigned int size, void* location) {

		alloc_t* current = head;

		if(current == NULL) {
			current = calloc( 1, sizeof(alloc_t));
			current->size = 0;
			current->location = NULL;
			current->next = NULL;
		}

		if( current->next != NULL)
			current = current->next;

		alloc_t* temp = calloc(1, sizeof(alloc_t));	
		temp->size = size;
		temp->location = location;
		temp->next = NULL;

		current->next = temp;
		
}

void remove_alloc(alloc_t* head, long* remove_me) {
	
	if(head == NULL) {
		return;
	}
	alloc_t* current = head;
	alloc_t* previous = current;
	while( current->location != remove_me) {
		previous = current;
		current = current->next;
		
	}
	alloc_t* temp = current;
	previous->next = current->next;
	current = current->next;
	free(temp);
}

void print_list(alloc_t* head) {
	unsigned int total_leaks = 0;
	alloc_t* temp = head;
	if(temp == NULL) {
		printf("head returned null\n");
		return;
	}
	while( temp->next != NULL) {
		temp = temp->next;
		printf("LEAK\t%u\n",temp->size);
		total_leaks += temp->size;
	}	
}

	alloc_t* my_allocs = NULL;
	static int total_allocs=0;
	
void* (*original_malloc)(size_t) = NULL;
void (*original_free)(void*) = NULL;

//called when the library is unloaded
void cleanup(void) {
	//use this to function to close any files that were still open
	//or do any other clean up.
}

void malloc_init(void) {
	if(original_malloc == NULL) {
		original_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	printf("initializing malloc.\n");
}
// intecepting malloc calls 
void* malloc(size_t size) {    
    
    void *heap = original_malloc(size);    
    
    if(heap) {    
		
		add_alloc(my_allocs, size, heap);		
		++total_allocs;
    }    
    
    return heap;    
}    

void free_init(void) {
	if(original_free == NULL) {
		original_free = dlsym(RTLD_NEXT, "free");
	}
	printf("initializing free.\n");
}	
//intercepting free calls
void free(void* temp) {
	remove_alloc(my_allocs, temp);
	print_list(my_allocs);
	--total_allocs;
	original_free(temp);	
}

















