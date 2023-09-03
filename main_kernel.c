#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/sched/signal.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");


void find_process_info(void)
{
    struct task_struct *task;

    for_each_process(task)
    {

        const char *str1 = task->comm;
        const char *str2 = "firefox";
        
        if(!(strcmp(str1, str2)))
        {
            printk(KERN_INFO "%s [%d]\n", task->comm, task->pid);
            break;
        }


    }
}

static int start_process(void)
{
    printk(KERN_INFO "Starting Process.\n");

    find_process_info();
    
    return 0;

}

static void exit_process(void)
{
    printk(KERN_INFO "Exiting Process.\n");
}

module_init(start_process);
module_exit(exit_process);