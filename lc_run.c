#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {


	//make a buffer that will be fed to the pipe
	char* cla[argc-1];
	cla[0] = "LD_PRELOAD=./leakcount.so ";
	//catch all cla
	if(argc > 1) {
		int i = 1;
		while( i < argc) {
			cla[i] = argv[i];
			++i;
		}
	}

	char shim_me[256] = {};
	for(int i = 0; i < argc; i++) {
			
			strcat(shim_me, cla[i]);	
	}

	FILE* pipe = popen(shim_me,"w"); 	
	if(!pipe)
		printf("popen returned null\n");	
	fflush(stdout);
	pclose(pipe);

	return 1;
	
}
