#define SHRINK_SIGNAL 0
#define STRETCH_SIGNAL 1

#define SIZE_SIG_BUFFER 100

struct PredictionFailureSignal{
    long task_id;
    long process_id;
    long tick;
    short type_signal;
};

void preempt_cur_task(struct PBS_Plan* p);
void add_signal(struct PredictionFailureSignal sig);
struct PredictionFailureSignal* get_pbs_signal(int pick_signal);
long find_slot_to_move_to(long target_pid, struct PBS_Plan* p);
void move_preempted_tasks(long insertion_slot, int stack_size, struct PBS_Task* preempted_tasks,struct PBS_Plan *p);
void signal_t2(struct PBS_Plan* p);
void signal_tm2(struct PBS_Plan* p);
void move_other_tasks_forward(long insertion_slot, long stack_size, struct PBS_Plan *p);
