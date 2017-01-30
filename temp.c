#include <stdio.h>
int main(void) {

	char* buf1 = malloc(2*sizeof(char));
	char* buf2 = malloc(2*sizeof(char));
	free(buf2);
	free(buf1);
	return 0;
}
