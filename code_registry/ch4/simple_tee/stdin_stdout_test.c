/*Test stdin/stdout in Linux*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef BUF_SIZ
#define BUF_SIZ 1024
#endif

 char buf[BUF_SIZ + 1];
 int numRead; 

int main(int argc, char *argv[])
{
    numRead =  read(STDIN_FILENO, buf, BUF_SIZ);
    printf("Number Read: %d\n", numRead);

    int res = write(STDOUT_FILENO, buf, numRead);
    printf("Number Write: %d\n", res);
    return 0;
}





