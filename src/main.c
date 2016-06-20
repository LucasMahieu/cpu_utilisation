#include <stdio.h>
#include <unistd.h>
#include "cpu_utilization.h"

int main(void){
	
	int i=0;
	int core_num = sysconf(_SC_NPROCESSORS_ONLN);
	//int* u = (int*)(calloc(core_num,sizeof(*u)));
	int* u;
	printf("Number of core = %d\n",core_num);
	int unit_value = sysconf(_SC_CLK_TCK);
	printf("Unit value = 1/%dsec.\n",unit_value);
	u = get_cpu_utilization(core_num);
	printf("Utilization of All cpu=%d%%\n",u[i]);
	for(i=1;i<core_num+1;++i){
		printf("---> Utilization of cpu%d=%d%%\n",i-1,u[i]);
	}
	free(u);
	return 0;
}











