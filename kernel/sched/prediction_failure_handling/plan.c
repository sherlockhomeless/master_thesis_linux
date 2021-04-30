#include <linux/kernel.h>

#include "pbs_entities.h"
#include "plan.h"
#include "task.h"
#include "process.h"
#include "prediction_failure_config.h"
// --- Task States ---
#define ON_TIME 0
#define TURNS_LATE 1
#define IS_LATE 2

void update_free_space_usage(long, struct PBS_Plan *);
short does_task_turn_late(long instructions_to_run, struct PBS_Task *task);

/**
 * Tracks the state changes of a task in terms of lateness
 * @param instructions_to_run Instructions run on the task
 * @param task
 * @return Task-States, see in defines
 */
short does_task_turn_late(long instructions_to_run, struct PBS_Task* task){
    if (task->instructions_retired_slot >= task->instructions_planned){
        return IS_LATE;
    }
    if (task->instructions_retired_slot + instructions_to_run > task->instructions_planned)
        return TURNS_LATE;
    else
        return ON_TIME;
}

EXPORT_SYMBOL(does_task_turn_late);

/**
 * Updates all data structures that are required for threshold tracking and calculating, also updates the state of a task accordingly
 * @param instructions_retired PMU-Counter since it was last read
 * @param p
 */
void update_retired_instructions(long instructions_retired, struct PBS_Plan* p){
    struct PBS_Task * slot_owner;
    short task_state;
    if (instructions_retired < 0){
        update_free_space_usage(instructions_retired, p);
        return;
    }

    task_state = does_task_turn_late(instructions_retired, p->cur_task);

    // --- update on tasks ---
    pbs_update_retired_instructions_task(instructions_retired, p->cur_task);

    // --- update retired instructions on slot ---
    if (p->cur_task->slot_owner == SHARES_NO_SLOT){
        p->cur_task->instructions_retired_slot += instructions_retired;
    } else {
        slot_owner = find_task_with_task_id(p, p->cur_task->slot_owner);

        slot_owner->instructions_retired_slot += instructions_retired;
    }

    // --- update  instructions on process/p ---
    update_retired_instructions_process(instructions_retired, p->cur_process);
    p->instructions_retired += instructions_retired;
}

/**
 * Updates the pointer cur_task & cur_process to reflect current state
 * @param p
 */
void update_cur_task_process(struct PBS_Plan *p) {
    change_task_state(p->cur_task, PLAN_TASK_RUNNING);
    if (p->cur_task->task_id != -1)
        p->cur_process = &p->processes[p->cur_task->process_id];
    else
        p->cur_process = &p->processes[MAX_NUMBER_PROCESSES -1];
    printk(KERN_INFO "[PBS_update_cur_task_process]%ld: (%ld,%ld) is new cur_task\n", p->tick_counter,
           p->cur_task->task_id, p->cur_task->process_id);
}

/**
 * Helps find a task in p->tasks with given task_id
 * @param p
 * @param task_id
 * @return
 */
struct PBS_Task* find_task_with_task_id(struct PBS_Plan* p, long task_id){
    struct PBS_Task* cur_task = p->cur_task;
    while (cur_task->task_id != -2){
        if (cur_task->task_id == task_id){
            return cur_task;
        }
        cur_task++;
    }
    return 0;
}
/**
 * Incorporates free space into lateness calculation for node lateness
 * @param length_free_space
 * @param p
 */
void update_free_space_usage(long length_free_space, struct PBS_Plan* p){
    p->lateness += length_free_space;
    if(LOG_PBS){
        printk(KERN_INFO "[PBS_update_free_space_usage]%ld, Node-lateness changed from %ld to %ld",p->tick_counter, p->lateness + length_free_space, p->lateness );
    }
}

void update_node_lateness(long instructions, struct PBS_Plan* p){
    p->lateness += instructions;
}
void change_plan_state(struct PBS_Plan* p, short state){
    p->state = state;
    printk(KERN_INFO"[PBS_change_plan_state]%ld: changed state %d\n", p->tick_counter, p->state);
}

void fill_empty_test_plan(struct PBS_Plan* p){
    p->cur_task = &p->tasks[0];
    p->finished_tasks = &p->tasks[0];
    p->cur_process = &p->processes[p->cur_task->process_id];
}
