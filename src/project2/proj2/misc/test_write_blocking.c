#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>


int main(int argc, char *argv[]) {

    printf("pid = %ld\n", (long)getpid());
    int cnt = 0;
    while (1)
    {
        int ret =  printf("this is great, cnt  = %d \n" , cnt);
        if (ret < 0)
            cnt++;
        else cnt = 0;
        sleep(1);
    }
    return 0;
}