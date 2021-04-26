#include "prediction_failure_config.h"

// ----- STRUCT DECLARATION -----

// TASK

#define PLAN_TASK_WAITING 0
#define PLAN_TASK_RUNNING 1
#define PLAN_TASK_PREEMPTED 2
#define PLAN_TASK_SIGNALED 3
#define PLAN_TASK_FINISHED 4
#define PLAN_TASK_ABORTED 5

#define SHARES_NO_SLOT 0


#define ON_PLAN 0
#define SIGNALED 1
#define PLAN_FINISHED 2



 struct PBS_Task {
    long task_id;  // task identifier
    long process_id; // pid of process task belongs to
    long instructions_planned; // instructions planned for task
    long instructions_real; // helper variable for simulating task deviations
    long instructions_retired_task; // instructions run on task
    long instructions_retired_slot; // instructions run on original slot of task
    short state; // state of task, see defines for possible values
    long lateness; // lateness of task
    long slot_owner; // ID of task in which current task runs; should be pointer for efficiency, but none trivial with task preemption and moving of tasks in memory
};

// PROCESS

 struct PBS_Process {
    long process_id;
    long num_tasks_remaining; //todo: update
    long buffer;
    long lateness;
    long length_plan;
    long instructions_retired;
};


// PLAN

 struct PBS_Plan{
    long num_processes;
    long num_tasks;
    struct PBS_Process processes[MAX_NUMBER_PROCESSES] ;
    struct PBS_Task tasks[MAX_NUMBER_TASKS_IN_PLAN];
    struct PBS_Task* finished_tasks;
    struct PBS_Process* cur_process;
    struct PBS_Task* cur_task;
    long lateness;
    long instructions_retired;
    short state;
    long tick_counter;
    long tasks_finished;
    long stress;
};
