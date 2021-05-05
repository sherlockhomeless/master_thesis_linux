#include "sched.h"


struct PBS_Plan pbs_plan = {0};
struct PBS_Plan* pbs_plan_ptr = &pbs_plan;


struct PBS_Plan* get_pbs_plan(void){
    printk(KERN_INFO "[PBS_get_pbs_plan]: pbs_plan_address is %p\n", pbs_plan_ptr);
    if (pbs_plan_ptr == NULL)
        printk(KERN_ALERT "[PBS_get_pbs_plan]: Plan is null\n");
    return pbs_plan_ptr;
}
EXPORT_SYMBOL(get_pbs_plan);
