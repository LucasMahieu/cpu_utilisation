#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_SIZE_LINE 120
#define SAMPLING_TIME_WAIT 3

typedef struct {
	char cpu_num[5];
	uint32_t user_time;
	uint32_t nice_time;
	uint32_t system_time;
	uint32_t idle_time;
	uint32_t iowait_time;
	uint32_t irq_time;
	uint32_t softirq_time;
} cpu_time;

cpu_time get_cpu_time(char* line);

int* get_cpu_utilization(int nb_core);

