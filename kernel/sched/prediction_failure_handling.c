#include "sched.h"

PBS_Task global_pbs_task = {0};
EXPORT_SYMBOL(global_pbs_task);

void fuck_this_shit_around(){
    printk(KERN_DEBUG "SHIT IS WORKING");
}

EXPORT_SYMBOL(fuck_this_shit_around);

void inc_global_task_lateness(){
    global_pbs_task.lateness += 1;
}
EXPORT_SYMBOL(inc_global_task_lateness);
