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
    set_pb_plan_size(pb_rq, 6);
    set_pb_plan_entry(pb_rq, 0, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 1, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 2, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 3, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 4, 3000000000, 3000000000);
    set_pb_plan_entry(pb_rq, 5, 3000000000, 0);
    
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
