//
// Created by ml on 23.03.21.
//

#ifndef LEVEL2_PROCESS_H
#define LEVEL2_PROCESS_H

#endif //LEVEL2_PROCESS_H

void update_retired_instructions_process(long instructions,struct PBS_Process*  process);
void update_lateness_process(long late_instructions,struct PBS_Process* process);
void update_finished_tasks_in_process(struct PBS_Process* process);


short is_process_late(struct PBS_Process*);