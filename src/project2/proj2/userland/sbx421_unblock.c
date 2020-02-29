#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

#include <stdlib.h>

#define SYSCALL_NM 435

long do_syscall(pid_t proc, unsigned long nr) {
    return syscall(SYSCALL_NM, proc , nr);
}


int main(int argc, char *argv[]) {
    long rv;

    if (argc != 3)
    {
        printf("Usage <Process ID> <system call number>\n");
        exit(0);
    }
    unsigned long a , b;
    char *tst;

    a = atol(argv[1]);
    b = atol(argv[2]);

    rv = do_syscall(a, b);
    printf("a = %lu , b = %lu\n",a , b );
    if(rv >= 0) {
        perror("Process unblocked successfully");
    }
    else {
        perror("err:\n");
    }

    return 0;
}