#include "sched.h"


struct PBS_Plan pbs_plan ={0};


struct PBS_Plan* get_pbs_plan(void){
    return &pbs_plan;
}
EXPORT_SYMBOL(get_pbs_plan);

extern void print_from_subdir(void){

}
EXPORT_SYMBOL(print_from_subdir);
