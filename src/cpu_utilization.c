#include "cpu_utilization.h"

// Uncomment this line to print debug line 
//#define DEBUG

cpu_time get_cpu_time(char* line){
	cpu_time ret;
	
	if(line == NULL) return ret;

	line[strlen(line)-1] = '\0';
	sscanf(line,"%4c %u %u %u %u %u %u %u",ret.cpu_num, &ret.user_time,&ret.nice_time,&ret.system_time,&ret.idle_time,&ret.iowait_time,&ret.irq_time,&ret.softirq_time);
#ifdef DEBUG
	printf("The line readed is ! line = %s\n",line);
	printf("Parameters are:\n");
	printf("- cpu :%s\n",ret.cpu_num);
	printf("- user time:%u\n",ret.user_time);
	printf("- nice time:%u\n",ret.nice_time);
	printf("- sys time :%u\n",ret.system_time);
	printf("- idle time :%u\n",ret.idle_time);
	printf("- iowait time :%u\n",ret.iowait_time);
	printf("- irq time :%u\n",ret.irq_time);
	printf("- softirq time :%u\n",ret.softirq_time);
#endif
	return ret;
}

int* get_cpu_utilization(int num_core){
	static int cpu_idle=0;
	static int cpu_idle_prev=0;
	static int cpu_not_idle=0;
	static int cpu_not_idle_prev=0;

	int i=0,core=0;
	int* u = (int*)(calloc(num_core,sizeof(*u)));
	char** cpu_line_prev = (char**)(calloc(num_core+1,sizeof(*cpu_line_prev)));
	for(i=0;i<num_core+1;++i){
		cpu_line_prev[i] = (char*)calloc(MAX_SIZE_LINE,sizeof(**cpu_line_prev));
	}
	char** cpu_line = (char**)(calloc(num_core+1,sizeof(*cpu_line)));
	for(i=0;i<num_core+1;++i){
		cpu_line[i] = (char*)calloc(MAX_SIZE_LINE,sizeof(**cpu_line));
	}
	cpu_time time_prev, time;
	int total_not_idle=0, total_idle=0;
	int cpu_total=0, cpu_total_prev=0;

	// capture the /proc/stat ligne corresponding to CPU(num)
	FILE* f_prev = fopen("/proc/stat","r");
	for(i=0;i<num_core+1;++i){
		if(fgets(cpu_line_prev[i], MAX_SIZE_LINE, f_prev) == NULL){
			printf("Error to get line in /proc/stat");
			return NULL;
		}
	}
	fclose(f_prev);

	// Waiting SAMPLING_TIME_WAIT to make an other measure
	sleep(SAMPLING_TIME_WAIT);
	
	// capture the /proc/stat ligne corresponding to CPU(num)
	FILE* f = fopen("/proc/stat","r");
	for(i=0;i<num_core+1;++i){
		if(fgets(cpu_line[i], MAX_SIZE_LINE, f) == NULL){
			printf("Error to get line in /proc/stat");
			return NULL;
		}
	}
	fclose(f);

	for(core=0;core<num_core+1;++core){
		// Parse the ligne to fill the structure of time
		time_prev = get_cpu_time(cpu_line_prev[core]);
		// Compute the idle and NotIdle time
		cpu_not_idle_prev = time_prev.user_time + time_prev.nice_time + time_prev.system_time + time_prev.irq_time + time_prev.softirq_time;
		cpu_idle_prev = time_prev.idle_time + time_prev.iowait_time;
#ifdef DEBUG
		printf("PREV = NOT_IDLE:%u   -   IDLE:%u   -   NOT_IDLE+IDLE:%u\n",cpu_not_idle_prev,cpu_idle_prev,cpu_not_idle_prev+cpu_idle_prev);
#endif

		// Parse the ligne to fill the structure of time
		time = get_cpu_time(cpu_line[core]);
		// Compute the idle and NotIdle time
		cpu_not_idle = time.user_time + time.nice_time + time.system_time + time.irq_time + time.softirq_time;
		cpu_idle = time.idle_time + time.iowait_time;

#ifdef DEBUG
		printf("ACTUAL = NOT_IDLE:%u   -   IDLE:%u   -   NOT_IDLE+IDLE:%u\n",cpu_not_idle,cpu_idle, cpu_not_idle+cpu_idle);
#endif
		// we can compute the difference between the two screenshot : now and prev
		total_not_idle = cpu_not_idle - cpu_not_idle_prev;
		total_idle = cpu_idle - cpu_idle_prev;
#ifdef DEBUG
		printf("TOTAL = NOT_IDLE:%u   -   IDLE:%u\n",total_not_idle,total_idle);
#endif
		// So, utilization = 100*total_not_idle / (total_idle+total_not_idle)
		u[core]= (total_not_idle*100) / (total_not_idle + total_idle) ;
	}

	for(i=0;i<num_core+1;++i){
		free(cpu_line_prev[i]);
	}
	free(cpu_line_prev);
	for(i=0;i<num_core+1;++i){
		free(cpu_line[i]);
	}
	free(cpu_line);
	return u;
}
