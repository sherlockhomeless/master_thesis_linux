#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <../kernel/sched/sched.h>


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("PB Scheduler - Module that sets a proxy task and scheduling plan");

static int loop_thread_func(void *data)
{
        unsigned int c = 0;
        while (!kthread_should_stop()) {
                int a = 0;
                set_current_state(TASK_INTERRUPTIBLE);
                for (;a < 200000; a++){}
                schedule();
                c++;
        }
        return 0;
}

static void init_rq(struct pb_rq *pb_rq)
{
    set_pb_plan_size(pb_rq, 100);
    set_pb_plan_entry(pb_rq, 0, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 1, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 2, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 3, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 4, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 5, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 6, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 7, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 8, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 9, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 10, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 11, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 12, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 13, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 14, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 15, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 16, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 17, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 18, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 19, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 20, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 21, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 22, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 23, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 24, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 25, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 26, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 27, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 28, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 29, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 30, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 31, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 32, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 33, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 34, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 35, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 36, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 37, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 38, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 39, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 40, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 41, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 42, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 43, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 44, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 45, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 46, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 47, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 48, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 49, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 50, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 51, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 52, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 53, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 54, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 55, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 56, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 57, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 58, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 59, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 60, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 61, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 62, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 63, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 64, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 65, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 66, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 67, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 68, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 69, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 70, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 71, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 72, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 73, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 74, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 75, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 76, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 77, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 78, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 79, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 80, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 81, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 82, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 83, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 84, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 85, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 86, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 87, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 88, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 89, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 90, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 91, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 92, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 93, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 94, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 95, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 96, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 97, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 98, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 99, 3000000000, 0);
    
}

static int __init pb_client_init(void)
{
    struct task_struct *proxy_task;
    struct rq *rq;

    proxy_task = kthread_create(loop_thread_func, NULL, "PB proxy thread");
    proxy_task->sched_class = &pb_sched_class;

    rq = this_rq();
    init_rq(&rq->pb);

    rq->pb.proxy_task = proxy_task;

    return 0;
}

static void __exit pb_client_cleanup(void)
{
    struct rq *rq;
    rq = this_rq();

    // set pb_rq back to initial values
    init_pb_rq(&rq->pb);

    printk(KERN_DEBUG "Cleaning up module.\n");
}

module_init(pb_client_init);
module_exit(pb_client_cleanup);
