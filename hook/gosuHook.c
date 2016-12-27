/**
*  @author : abhishek goswami
*  abhishek goswami
*
*  hook ! hook ! hook ! the system call :P
*/

// finding the sys_call_table address : /boot/System.map-<version> | grep sys_call_table
// i will be hard coding the sys_call_table into the programming , well there are other ways too.

#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscall.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ABHISHEK_GOSWAMI");
/* my sys call table address */
//c153e180 

void **system_call_table_addr;

/* my custom syscall that takes process name */
asmlinkage int (*custom_syscall) (char *name);

/* hook */
asmlinkage int gosu_hook(char *play_here){

/* do whatever here , for now i will just be printing to the demsg log
*/

printk(KERN_INFO "Pname syscall: Hook! Hook! Hook!");
return custom_syscall(play_here);

}

/* make page writable */
int make_rw(unsigned long address){

    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW){
        pte->pte |=_PAGE_RW;
    }

    return 0;

}

/* Make the page write protected */
int make_ro(unsigned long address){

    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    return 0;

}

static int __init entry_point(void) {
    printk(KERN_INFO "Gosu_Hook loaded successfully!!!");
    system_call_table_addr = (void*)c153e180;
    
    /* replace custom syscall with correct system call name (write, open , etc) to hook */
    custom_syscall = system_call_table_addr[__NR_pname];
    
    /* disable page protection */
    make_rw((unsigned long)system_call_table_addr);
    
    /* change syscall to our syscall function */
    system_call_table_addr[__NR_pname] = gosu_hook;
    return 0;
    
}

static int __exit exit_point(void) {
     printk(KERN_INFO "Unloaded Gosu_Hook successfully\n");

    /*  restore original sys call */
    system_call_table_addr[__NR_pname] = custom_syscal;

    /* Renable page protection */
    make_ro((unsigned long)system_call_table_addr);
    return 0;
}

module_init(entry_point);
module_exit(exit_pointw);
