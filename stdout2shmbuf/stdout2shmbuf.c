#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <signal.h>

#define SHM_FNAME "rbuff"
#define XFER_SIZE 4096
#define BUFFDEPTH 512

static volatile int run_var = 1;

void inter_handler(int dummy) 
{
    run_var = 0;
}

int main()
{
    int fd;
    unsigned int buff_count=0;
    u_char *memaddr;

    fd = shm_open(SHM_FNAME, O_RDWR|O_CREAT, 0664);
    assert(fd!=-1);
    ftruncate(fd, BUFFDEPTH*XFER_SIZE);
    memaddr = (u_char*)mmap(0, BUFFDEPTH*XFER_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    //printf("%p\n", memaddr);
    signal(SIGINT, inter_handler);
    while (run_var)
    { 
        read(STDIN_FILENO, memaddr+(buff_count*XFER_SIZE), XFER_SIZE);
        buff_count = (buff_count+1)%BUFFDEPTH;
    }
    printf ("Signal caught, exiting the stdout2shmbuf code \n");
    munmap(memaddr, BUFFDEPTH*XFER_SIZE);
    shm_unlink(SHM_FNAME);
    close(fd);
    return 0;
}
