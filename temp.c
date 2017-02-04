#include <stdio.h>
int main(void) {

	char* buf1 = malloc(10*sizeof(char));
	char* buf2 = malloc(20*sizeof(char));
	char* buf3 = malloc(30*sizeof(char));
	char* buf4 = malloc(40*sizeof(char));
	char* buf5 = malloc(50*sizeof(char));
	char* buf6 = malloc(60*sizeof(char));
/*
	printf("\tbuf1 location is %p\n\n", buf1);
	printf("\tbuf2 location is %p\n\n", buf2);
	printf("\tbuf3 location is %p\n\n", buf3);
*/
	free(buf2);
	return 0;
}
