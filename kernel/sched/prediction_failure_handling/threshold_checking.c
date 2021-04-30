#include <linux/kernel.h>
//
// Created by ml on 26.03.21.
//


#include "pbs_entities.h"
#include "plan.h"
#include "threshold_checking.h"
#include "prediction_failure_config.h"


long calculate_t1(struct PBS_Task*);
long calculate_t2_task( struct PBS_Plan *p);

long calculate_t1(struct PBS_Task* task){
    long t1_relative, t1_minimum, t1_maximum, t1;
    t1_minimum = task->instructions_planned + NO_PREEMPTION;
    t1_relative = task->instructions_planned * SIGMA_T1;
    t1_maximum = task->instructions_planned + NO_PREEMPTION;
    if(0)
        printk("[t1] task %ld: instructions_planned=%ld, t1_min=%ld, t1_relative=%ld, t1_max=%ld\n", task->task_id,
               task->instructions_planned, t1_minimum, t1_relative, t1_maximum);
    if (t1_minimum > t1_relative)
        t1_relative = t1_minimum;
    if (t1_maximum > t1_relative) {
        t1 = t1_relative;
    } else {
        t1 = t1_maximum;
    }
    return t1;
}

EXPORT_SYMBOL(calculate_t1);

short check_t1(struct PBS_Plan* p) {
    struct PBS_Task* task_to_check;
    long t1;

    if(!T1_ENABLED) {
        return OK;
    }
    //task was moved and is not in original slot anymore
    if (p->cur_task->slot_owner == SHARES_NO_SLOT){
        //todo: stack check, is >1 one task in new slot
        task_to_check = p->cur_task;
    } else {
        task_to_check =  find_task_with_task_id(p, p->cur_task->slot_owner);
    }
     t1 = calculate_t1(task_to_check);
    if(task_to_check->instructions_retired_slot >= t1){
        return T1;
    } else {
        return OK;
    }
}

EXPORT_SYMBOL(check_t1);

long calculate_t2_task(struct PBS_Plan *p) {
    struct PBS_Task* task = p->cur_task;
    long t2_task, t2_task_min, t2_task_relative, t1;
    t1 = calculate_t1(task);
    t2_task_min = (long) (t1 + T2_SPACER);
    t2_task_relative = (long) (task->instructions_planned * CAP_LATENESS);
    if(LOG_PBS)
        printk(KERN_INFO "[PBS_calculate_t2_task] task %ld: instructions_planned=%ld, t2_task_min=%ld, t2_task_relative=%ld\n",
               task->task_id, task->instructions_planned, t2_task_min, t2_task_relative);

    if (t2_task_min > t2_task_relative)
        t2_task = t2_task_min;
    else
        t2_task = t2_task_relative;

    return t2_task;
}

EXPORT_SYMBOL(calculate_t2_task);

short check_t2_task( struct PBS_Plan *p) {
    long t2_task;
    if(!T2_TASK_ENABLED)
        return OK;
    t2_task = calculate_t2_task(p);
    // compare
    if (p->cur_task->instructions_retired_slot >= t2_task)
        return T2;
    else
        return OK;
}

EXPORT_SYMBOL(check_t2_task);

short check_tm2_task(struct PBS_Plan* p){
    struct PBS_Task* task = p->cur_task;
    long plan_length = task->instructions_planned;
    long tm2_task = (plan_length * CAP_LATENESS/10) / 100;
    if (!TM2_TASK_ENABLED) {
        return OK;
    }

    if (task->instructions_retired_slot < tm2_task && task->state == PLAN_TASK_FINISHED)
        return TM2;
    else
        return OK;
}

EXPORT_SYMBOL(check_tm2_task);

short check_t2_process(struct PBS_Plan* p) {
    if (!T2_PROCESS_ENABLED)
        return OK;
    // TODO: HIER WEITER
    return OK;
}

EXPORT_SYMBOL(check_t2_process);

short check_tm2_process(struct PBS_Plan* p) {
    if (!TM2_PROCESS_ENABLED)
        return OK;
    return OK;
}

EXPORT_SYMBOL(check_tm2_process);

/**
 * Calculates the usable_buffer, float-values have to be given as integers because of kernel limitations
 * @param free_time freetime as integer, eg. 10 => 10% free time
 * @param assignable_buffer
 * @param buffer
 * @param length_process_plan
 * @param length_process_finished
 * @return usable_buffer in instructions
 */
long calculate_usable_buffer(int free_time, int assignable_buffer, long buffer, long length_process_plan, long length_process_finished){
    long buffer_with_free_time_applied;
    long available_buffer;
    long usable_buffer;
    long process_progress;

    buffer_with_free_time_applied =  (buffer * free_time / 100) ;
    available_buffer = buffer_with_free_time_applied * assignable_buffer / 100;
    process_progress = (length_process_finished * ACCURACY) / length_process_plan;

    usable_buffer = available_buffer * process_progress /ACCURACY;

    return usable_buffer;
}

EXPORT_SYMBOL(calculate_usable_buffer);

short check_t2_node(struct PBS_Plan* plan){
    if (!T2_NODE_ENABLED)
        return OK;
    return OK;
}

EXPORT_SYMBOL(check_t2_node);

short check_tm2_node(struct PBS_Plan* plan){
    if (!TM2_NODE_ENABLED)
        return OK;
    return OK;
}

EXPORT_SYMBOL(check_tm2_node);
