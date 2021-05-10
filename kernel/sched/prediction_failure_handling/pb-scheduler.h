//
// Created by ml on 26.03.21.
//

#include "plan.h"
#include "pbs_entities.h"

#ifndef LEVEL2_PB_SCHEDULER_H
#define LEVEL2_PB_SCHEDULER_H

#endif //LEVEL2_PB_SCHEDULER_H

struct PBS_Plan* get_pbs_plan(void);
// --- interface ---
void pbs_handle_prediction_failure(struct PBS_Plan *p);
void start_run(struct PBS_Plan *p);


