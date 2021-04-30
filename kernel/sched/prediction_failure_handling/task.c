#include <linux/kernel.h>
//
// Created by ml on 25.03.21.
//

#include "pbs_entities.h"
#include "task.h"
#include "prediction_failure_handling.h"

void pbs_update_retired_instructions_task(long instructions, struct PBS_Task*task) {
    task->instructions_retired_task += instructions;

    // update lateness
    if (task->instructions_retired_task > task->instructions_planned){
        task->lateness = task->instructions_retired_task - task->instructions_planned;
    }
    // check if finished
    if (task->instructions_retired_task >= task->instructions_real){
        task->lateness = task->instructions_real - task->instructions_planned;
        change_task_state(task, PLAN_TASK_FINISHED);
    }
}

EXPORT_SYMBOL(pbs_update_retired_instructions_task);

void change_task_state(struct PBS_Task *t, short state) {
    short state_before = t->state;
    t->state = state;
    printk(KERN_INFO "[change_task_state] changed task %ld from %d to %d\n", t->task_id, state_before, state);
}
EXPORT_SYMBOL(change_task_state);
