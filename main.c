#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

pid_t my_pid;

static int start_process(void)
{
    printk(KERN_INFO "Starting Process.\n");
    return 0;

}

static void exit_process(void)
{
    printk(KERN_INFO "Exiting Process.\n");
}

module_init(start_process);
module_exit(exit_process);