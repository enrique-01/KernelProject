#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/sched/signal.h>
#include <linux/string.h>
#include <linux/mm_types.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");


pid_t pid; //need to change from global if using concurrency eventually
struct mm_struct *mm;

void find_process_info(void)
{
    struct task_struct *task;

    for_each_process(task)
    {
        const char *str1 = task->comm;
        const char *str2 = "firefox";
        
        if(!(strcmp(str1, str2)))
        {
            //printk(KERN_INFO "%s [%d]\n", task->comm, task->pid);
            mm = task->mm;
            break;
        }

    }

}

void read_from_process(void)
{
    struct vm_area_struct *vma = mm->mmap;
    unsigned long len = vma->vm_end - vma->vm_start;
    int ret;

    ret = remap_pfn_range(vma, vma->vm_start, 1, len, vma->vm_page_prot);
    if(ret < 0)
    {
        printk(KERN_INFO "Could not map the address area!\n");
    }

}

static int start_process(void)
{
    printk(KERN_INFO "Starting Process.\n");

    find_process_info();
    read_from_process();

    //printk(KERN_INFO "PID: %d \n", pid);
    
    
    return 0;

}

static void exit_process(void)
{
    printk(KERN_INFO "Exiting Process.\n");
}

module_init(start_process);
module_exit(exit_process);