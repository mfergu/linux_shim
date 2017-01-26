#include "leakcount.h"

int main(int argc, char* argv) {

	int size = 0;
	char * buff = 0;
	
	printf("please enter a size: ");
	
	fflush(stdout);
	
	scanf("%d", &size);
	
	buff = malloc (size);
	
	free(buff);

	return;
	
}
