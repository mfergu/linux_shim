#include <stdio.h>
int main(void) {
printf("/n/n/t this is temp\n\n");

	int val = 1;
	char* buf = malloc(val);
	free(buf);
	return 0;
}
