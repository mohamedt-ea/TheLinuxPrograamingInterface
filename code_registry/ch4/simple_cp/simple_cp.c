/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2019.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 4-1 */

/* copy.c

   Copy the file named argv[1] to a new file named in argv[2].
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>    /* Prototypes for many system calls */
#include <errno.h>     /* Declares errno and defines error constants */
#include <string.h>    /* Commonly used string-handling functions */
#include <stdbool.h>   /* 'bool' type plus 'true' and 'false' constants */

#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    printf("PID: %d\n", getpid());

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("%s old-file new-file\n", argv[0]);

    /* Open input and output files */
    inputFd = open(argv[1], O_RDONLY);
    printf("File Descriptor for input: %d\n", inputFd);
    if (inputFd == -1)
        printf("opening file %s", argv[1]);
    getchar();
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);

    printf("File Descriptor For output: %d\n", outputFd);
    getchar();
    if (outputFd == -1)
        printf("opening file %s", argv[2]);

    /* Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            printf("couldn't write whole buffer");
    if (numRead == -1)
        printf("read");

    if (close(inputFd) == -1)
        printf("close input");
    if (close(outputFd) == -1)
        printf("close output");

    exit(EXIT_SUCCESS);
}
