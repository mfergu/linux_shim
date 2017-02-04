#include <stdio.h>
int main(void) {

	char* buf1 = malloc(10*sizeof(int));
	char* buf2 = malloc(20*sizeof(int));
	char* buf3 = malloc(30*sizeof(int));
	char* buf4 = malloc(40*sizeof(char));
	char* buf5 = malloc(50*sizeof(char));
	char* buf6 = malloc(60*sizeof(char));
	char* buf7 = calloc(70, sizeof(int));
	char* buf8 = calloc(80, sizeof(int));
/*
	printf("\tbuf1 location is %p\n\n", buf1);
	printf("\tbuf2 location is %p\n\n", buf2);
	printf("\tbuf3 location is %p\n\n", buf3);
*/
	free(buf7);
	free(buf8);
	free(buf1);
	free(buf6);
	return 0;
}
