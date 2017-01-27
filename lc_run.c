#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	//take second command line argument add LD_PRELOAD of shimmed malloc
	char* ld_preload = "LD_PRELOAD=./leakcount.so ./";
	char shim_me[256];
	snprintf(shim_me, sizeof shim_me, "%s%s", ld_preload, argv[1]);
	FILE* pipe = popen(shim_me,"r"); 	
	if(!pipe)
		printf("popen returned null\n");	
	int size = 0;
	char* buff = 0;
	fflush(stdout);
	buff = malloc (size);
	free(buff);

	return 1;
	
}
