/*
*  author : abhishek goswami ( hiro )
*  abhishekg785@gmail.com
*/

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched>
#include <linux/init.h>
#include <linux/tty.>
#include <linux/string.h>

#include "pname.h"

asmlinkage long sys_process_name(char* process_name){
    
    /* tasklist struct to use */
    struct task_struct *task;
    
    /* tty struct */
    struct tty_struct *my_tty;
    
    /* get current tty */
    my_tty = get_current_tty();
    
    /* placeholder to print full string to tty */
    char name[32];
    
    /* <sched.h> library method  that iterates the  list of processes from task_struct defined above */
    for_each_process(task){

        /* compares the current process name ( defined in task->comm ) with the passed in name */
        if(strcmp(task->comm, process_name) == 0){

            /* convert to string and put into name[] */
            sprintf(name, "PID=%ld\n", (long)task_pid_nr(task));

            /* show result to the user who called the syscall */
            ( my_tty->driver->ops->write ) (my_tty, name, strlen(name) + 1);
        }
    }
    return 0;
}

