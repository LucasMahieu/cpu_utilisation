#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_SIZE_LINE 100
#define SAMPLING_TIME_WAIT 1

typedef struct {
	char* cpu_num;
	uint32_t user_time;
	char* nice_time;
	char* system_time;
	char* idle_time;
	char* iowait_time;
	char* irq_time;
	char* softirq_time;
	char* steal_time;
	char* guest_time;
	char* guestnice_time;
} cpu_time;

cpu_time get_cpu_time(char* line){
	cpu_time ret;
	
	if(line == NULL) return ret;
	
	sscanf(line,"%s  %d %s %s %s %s %s %s %s %s %s",&ret.cpu_num, &ret.user_time,&ret.nice_time,&ret.system_time,&ret.idle_time,&ret.iowait_time,&ret.irq_time,&ret.softirq_time,&ret.steal_time,&ret.guest_time,&ret.guestnice_time);
	return ret;
}

int main(void){
	FILE* f = fopen("/proc/stat","r");
	
	static int cpu_idle=0;
	static int cpu_idle_prev=0;
	static int cpu_not_idle=0;
	static int cpu_not_idle_prev=0;

	cpu_time var;

	char cpu_line[MAX_SIZE_LINE];

	if(fgets(cpu_line, MAX_SIZE_LINE, f) == NULL){
		printf("Error to get line in /proc/stat");
		return -1;
	}
	
	var = get_cpu_time(cpu_line);

	printf("cpu user time  = %d\n",var.user_time);
	printf("line = %s",cpu_line);

	sleep(SAMPLING_TIME_WAIT);

	if(fgets(cpu_line, MAX_SIZE_LINE, f) == NULL){
		printf("Error to get line in /proc/stat");
		return -1;
	}

	var = get_cpu_time(cpu_line);

	printf("cpu time  = %d\n",var.user_time);
	printf("line = %s",cpu_line);

	return 0;
}











