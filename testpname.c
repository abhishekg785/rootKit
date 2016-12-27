/**
*  @author : abhishek goswami ( hiro )
*  abhishekg785@gmail.com
*
*  testpname.c  : to test the syscall 'pname' made by us :)
*/

#include<stdio.h>
#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<string.h>

int main() {
    char name[32];
    puts("enter the process to find");
    scanf("%d",name);
    strtok(name, "\n");
    // 317 here is the no at which my  module for syscall was loaded
    long int status = syscall(317, name);  // syscall no 317 and passing in the string 
    
    printf("System call returned $ld\n", status);
    return 0;
}
