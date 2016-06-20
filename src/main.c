#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_SIZE_LINE 100
#define SAMPLING_TIME_WAIT 2

typedef struct {
	char cpu_num[5];
	uint32_t user_time;
	uint32_t nice_time;
	uint32_t system_time;
	uint32_t idle_time;
	uint32_t iowait_time;
	uint32_t irq_time;
	uint32_t softirq_time;
	uint32_t steal_time;
	uint32_t guest_time;
	uint32_t guestnice_time;
} cpu_time;

cpu_time get_cpu_time(char* line){
	cpu_time ret;
	
	if(line == NULL) return ret;
	
	sscanf(line,"%s %d %d %d %d %d %d %d %d %d %d",&ret.cpu_num, &ret.user_time,&ret.nice_time,&ret.system_time,&ret.idle_time,&ret.iowait_time,&ret.irq_time,&ret.softirq_time,&ret.steal_time,&ret.guest_time,&ret.guestnice_time);
	return ret;
}

int get_cpu_utilization(int num){
	static int cpu_idle=0;
	static int cpu_idle_prev=0;
	static int cpu_not_idle=0;
	static int cpu_not_idle_prev=0;

	char cpu_line[MAX_SIZE_LINE];
	cpu_time time_prev, time;
	int computed_total_not_idle=0, computed_total_idle=0;
	int total=0, total_prev=0;
	int i=0;

	// capture the /proc/stat ligne corresponding to CPU(num)
	FILE* f = fopen("/proc/stat","r");
	for(i=-1;i<num;++i){
		if(fgets(cpu_line, MAX_SIZE_LINE, f) == NULL){
			printf("Error to get line in /proc/stat");
			return -1;
		}
	}
	fclose(f);
	// Parse the ligne to fill the structure of time
	time_prev = get_cpu_time(cpu_line);
	// Compute the idle and NotIdle time
	cpu_not_idle_prev = time_prev.user_time + time_prev.nice_time + time_prev.system_time + time_prev.irq_time + time_prev.softirq_time + time_prev.steal_time + time_prev.guestnice_time + time_prev.guest_time;
	cpu_idle_prev = time_prev.idle_time + time_prev.iowait_time;
	total_prev = cpu_idle_prev + cpu_not_idle_prev;

	// Waiting SAMPLING_TIME_WAIT to make an other measure
	sleep(SAMPLING_TIME_WAIT);

	// capture the /proc/stat ligne corresponding to CPU(num)
	f = fopen("/proc/stat","r");
	for(i=-1;i<num;++i){
		if(fgets(cpu_line, MAX_SIZE_LINE, f) == NULL){
			printf("Error to get line in /proc/stat");
			return -1;
		}
	}
	fclose(f);
	// Parse the ligne to fill the structure of time
	time = get_cpu_time(cpu_line);
	// Compute the idle and NotIdle time
	cpu_not_idle = time.user_time + time.nice_time + time.system_time + time.irq_time + time.softirq_time + time.steal_time + time.guestnice_time + time.guest_time;
	cpu_idle = time.idle_time + time.iowait_time;
	total = cpu_idle + cpu_not_idle;

	// on peut alors faire la diff entre now and prev
	computed_total_not_idle = total - total_prev;
	computed_total_idle = cpu_idle - cpu_idle_prev;

	// So, utilization = computed_total_not_idle - idle / computed_total_not_idle 
	return (computed_total_not_idle - computed_total_idle) / computed_total_not_idle;
}



int main(void){
	
	int u = 0;
	int i=0;
	int core_num = sysconf(_SC_NPROCESSORS_ONLN);
	u = get_cpu_utilization(0);
	printf("utilization of cpuTOTAL=%d%\n",u);
	for(i=1;i<core_num+1;++i){
		u = get_cpu_utilization(i);
		printf("utilization of cpu%d=%d%\n",i-1,u);
	}
	return 0;
}











