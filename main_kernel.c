#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/printk.h>
#include <linux/sched/signal.h>
#include <linux/string.h>
#include <linux/mm_types.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

static DEFINE_MUTEX(mm_mutex);

void read_from_process_memory(struct task_struct *task)
{
    struct mm_struct *mm = get_task_mm(task);
    if (mm != NULL)
    {
        struct vm_area_struct *vma;
        unsigned long addr;
        unsigned char *buffer;
        size_t size;
        int bytes_read;
        VMA_ITERATOR(iter, mm, 0);

        for_each_vma(iter, vma)
        {
            size = vma->vm_end - vma->vm_start;
            printk(KERN_INFO "Virtual Size: %zu\n", size);
            printk(KERN_INFO "VMA start: %lx, end: %lx, flags: %lx\n", vma->vm_start, vma->vm_end, vma->vm_flags);

            if (!(vma->vm_flags & VM_READ))
                continue;

            addr = vma->vm_start;
            buffer = kmalloc(size, GFP_KERNEL);

            if (buffer)
            {
                bytes_read = access_process_vm(task, addr, buffer, size, 0); // Use task instead of current
                printk(KERN_INFO "bytes_read: %i\n", bytes_read);
                if (bytes_read > 0)
                {
                    printk(KERN_INFO "Contents of process memory at address %lx:\n", addr);
                    print_hex_dump(KERN_INFO, "", DUMP_PREFIX_ADDRESS, 16, 1, buffer, bytes_read, true);
                }
                else
                {
                    printk(KERN_ERR "Failed to read process memory at address %lx\n", addr);
                }

                kfree(buffer);
            }
        }
        mmput(mm); // Properly release the mm_struct reference
    }
    else
    {
        printk(KERN_ERR "mm is NULL for task %d\n", task->pid);
    }
}

void find_and_process_firefox(void)
{
    struct task_struct *task;

    printk(KERN_INFO "Starting to find and process Firefox processes.\n");

    for_each_process(task)
    {
        const char *str1 = task->comm;
        const char *str2 = "firefox";  // Ensure the name matches correctly

        printk(KERN_INFO "Checking process: %s [%d]\n", task->comm, task->pid);

        if (!(strcmp(str1, str2)))
        {
            printk(KERN_INFO "Found process: %s [%d]\n", task->comm, task->pid);
            mutex_lock(&mm_mutex);
            read_from_process_memory(task);
            mutex_unlock(&mm_mutex);
        }
    }

    printk(KERN_INFO "Finished finding and processing Firefox processes.\n");
}

static int start_process(void)
{
    printk(KERN_INFO "Starting Process.\n");
    find_and_process_firefox();
    return 0;
}

static void exit_process(void)
{
    printk(KERN_INFO "Exiting Process.\n");
}

module_init(start_process);
module_exit(exit_process);
