#include <linux/kernel.h>
//
// Created by ml on 25.03.21.
//

#include "pbs_entities.h"
#include "process.h"

/**
 *
 * @param instructions instructions run on the process given
 */
void update_retired_instructions_process(long instructions, struct PBS_Process* process){
    process->instructions_retired += instructions;
}

EXPORT_SYMBOL(update_retired_instructions_process);


void update_lateness_process(long late_instructions, struct PBS_Process* process){
    process->lateness += late_instructions;
}

EXPORT_SYMBOL(update_lateness_process);

short is_process_late(struct PBS_Process* process){
    if(process->lateness < 0)
        return 0;
    else
        return 1;
}

EXPORT_SYMBOL(is_process_late);

void update_finished_tasks_in_process(struct PBS_Process* process){
    process->num_tasks_remaining--;
}

EXPORT_SYMBOL(update_finished_tasks_in_process);