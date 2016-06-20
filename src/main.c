#include <stdio.h>
#include <unistd.h>
#include "cpu_utilization.h"

int main(void){
	
	int u = 0;
	int i=0;
	int core_num = sysconf(_SC_NPROCESSORS_ONLN);
	printf("Number of core = %d\n",core_num);
	int unit_value = sysconf(_SC_CLK_TCK);
	printf("Unit value = 1/%dsec.\n",unit_value);
	u = get_cpu_utilization(0);
	printf("Utilization of All cpu=%d%%\n",u);
	for(i=1;i<core_num+1;++i){
		u = get_cpu_utilization(i);
		printf("---> Utilization of cpu%d=%d%%\n",i-1,u);
	}
	return 0;
}











