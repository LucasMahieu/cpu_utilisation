#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE_LINE 100

int main(void){
	FILE* f = fopen("/proc/stat","r");
	

	char cpu_line[MAX_SIZE_LINE];

	if(fgets(cpu_line, MAX_SIZE_LINE, f) == NULL){
		printf("Error to get line in /proc/stat");
		return -1;
	}

	printf("line = %s",cpu_line);
	return 0;
}
