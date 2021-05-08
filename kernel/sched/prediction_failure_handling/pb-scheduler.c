#include <linux/kernel.h>
//
// Created by ml on 26.03.21.
//

#include "pb-scheduler.h"
#include "pmu_interface.h"
#include "plan.h"
#include "process.h"
#include "threshold_checking.h"
#include "prediction_failure_handling.h"
#include "prediction_failure_config.h"

void update(long retired_instructions, struct PBS_Plan *p);
void schedule_task_finished(struct PBS_Plan*);
void schedule_timer_tick(struct PBS_Plan*);
void switch_task(struct PBS_Plan*);
void handle_free_slot(struct PBS_Plan*);

struct PBS_Plan pbs_plan = {0};
struct PBS_Plan* pbs_plan_ptr = &pbs_plan;

struct PBS_Plan* get_pbs_plan(void){
    printk(KERN_INFO "[PBS_get_pbs_plan]: pbs_plan_address is %p\n", pbs_plan_ptr);
    if (pbs_plan_ptr == NULL)
        printk(KERN_ALERT "[PBS_get_pbs_plan]: Plan is null\n");
    return pbs_plan_ptr;
}
EXPORT_SYMBOL(get_pbs_plan);

/**
 * Is called by the tick-function after each timer interrupt
 * @param p
 */
void schedule_pbs(struct PBS_Plan *p) {
    long retired_instructions;
    if (p->cur_task->task_id == -2){
        printk(KERN_INFO "[PBS_SCHEDULE]%ld finished running p ticks", p->tick_counter);
        change_plan_state(p, PLAN_FINISHED);
        return;
    }
    //todo: Just for checking:
    retired_instructions = get_retired_instructions();
    update(retired_instructions, p);

    if(p->cur_task->state == PLAN_TASK_FINISHED){ // schedule_pbs() was called after a task has finished
        schedule_task_finished(p);
    } else {
        schedule_timer_tick(p);
    }
    p->tick_counter++;
}
EXPORT_SYMBOL(schedule_pbs);


/**
 * Is called when a task has finished; checks for t-2 and updates stats
 * @param p
 */
void schedule_task_finished(struct PBS_Plan *p){
    long lateness_cur_task;
    long instruction_surplus = p->cur_task->instructions_retired_slot - p->cur_task->instructions_real; // take surplus of instructions attributed to cur_task and remove them
    // --- check ---
    // todo: include other threshold-checks
    // check_tm2_node(p) && check_tm2_process(p->cur_process) &&
    if ( check_tm2_task(p)){
        //TODO: signal tm2
        signal_tm2(p);
        printk(KERN_WARNING "[PBS_schedule_task_finished]%ld: PBS_Task%ld finished early\n",p->tick_counter, p->cur_task->task_id);
    } else {
        printk(KERN_INFO "[PBS_schedule_task_finished]%ld: PBS_Task%ld finished, planned: %ld, real: %ld, retired: %ld\n", p->tick_counter, p->cur_task->task_id,
               p->cur_task->instructions_planned, p->cur_task->instructions_real, p->cur_task->instructions_retired_slot);
    }

    // --- update ---
    lateness_cur_task = p->cur_task->instructions_real - p->cur_task->instructions_planned;
    update_lateness_process(lateness_cur_task, p->cur_process);
    update_node_lateness(lateness_cur_task, p);

    p->instructions_retired -= instruction_surplus;
    p->cur_process->instructions_retired -= instruction_surplus;
    p->cur_task->instructions_retired_slot = p->cur_task->instructions_real;

    p->stress--;
    switch_task(p);
}

EXPORT_SYMBOL(schedule_task_finished);


/**
 * Function is called when a timer tick happened; check if t1,t2 is transgressed
 * @param p
 */
void schedule_timer_tick(struct PBS_Plan *p){
    long usable_buffer;
    //todo: test stress
    //todo: preemptions
    // --- check for t2 ---
    if(p->stress <= 0) {
        p->state = ON_PLAN;
        usable_buffer = calculate_usable_buffer(FREE_TIME, ASSIGNABLE_BUFFER, p->cur_process->buffer,
                                                     p->cur_process->length_plan,
                                                     p->cur_process->instructions_retired);
        if (check_t2_task(p) ||
            check_t2_process(p) ||
            check_t2_node(p))            {
            signal_t2(p);
        }
    }

    // --- check for t1 ---
    if (check_t1(p)){
        preempt_cur_task(p);
        return;
    }
    if(LOG_PBS)
        printk(KERN_INFO "[PBS_schedule_timer_tick]%ld: (%ld,%ld) retired instructions %ld\n",
           p->tick_counter, p->cur_task->task_id, p->cur_task->process_id, p->cur_task->instructions_retired_slot);
    p->stress--;
}

EXPORT_SYMBOL(schedule_timer_tick);

/**
 * Implements a switching of tasks, changes the current Task for the Task in the next slot
 * @param p
 */
void switch_task(struct PBS_Plan* p){
    struct PBS_Task* old_task = p->cur_task;
    p->tasks_finished++;
    p->index_cur_task++;
    p->cur_task++;
    update_cur_task_process(p);
    if (p->cur_task->task_id == -1){
        handle_free_slot(p);
    }
    if(LOG_PBS)
        printk(KERN_INFO "[PBS_switch_task]%ld: switched from task %ld to task %ld in tick %ld \n", p->tick_counter, old_task->task_id, p->cur_task->task_id, p->tick_counter);
}

EXPORT_SYMBOL(switch_task);

/**
 * starts a run inside the kernel with the given plan
 * @param p plan to start a run
 */
void start_run(struct PBS_Plan *p){
    while (p->state != PLAN_FINISHED){
        schedule_pbs(p);
    }
    printk(KERN_EMERG "[PBS_start_run]%ld: Plan finished!", p->tick_counter);
}
EXPORT_SYMBOL(start_run);

/**
 * Needs to be called when next task is free slot:
 * - Picks next task
 * - Updates Lateness
 * @param p
 */
void handle_free_slot(struct PBS_Plan* p){
    long lateness_node_before,  lateness_node_after;
    struct PBS_Task* free_slot;


    printk(KERN_INFO "[PBS_handle_free_slot]%ld: Found free slot of length %ld\n", p->tick_counter, p->cur_task->instructions_planned);
    lateness_node_before = p->lateness;
    free_slot = p->tasks;
    p->index_cur_task++;
    p->cur_task++;
    update_cur_task_process(p);
    update_retired_instructions(- free_slot->instructions_planned, p);
    lateness_node_after = p->lateness;
}

EXPORT_SYMBOL(handle_free_slot);

/**
 * Updates ALL data structures to the state including the current tick
 * @param retired_instructions number of instructions retired on the execution of p-tasks since last tick
 * @param p current p
 */
void update(long retired_instructions, struct PBS_Plan* p){
    update_retired_instructions(retired_instructions, p);
    if(LOG_PBS)
        printk(KERN_INFO "[PBS_update]%ld: (%ld,%ld): instructions_retired_slot: %ld \n", p->tick_counter, p->cur_task->task_id, p->cur_task->process_id, p->cur_task->instructions_retired_slot);
}
EXPORT_SYMBOL(update);