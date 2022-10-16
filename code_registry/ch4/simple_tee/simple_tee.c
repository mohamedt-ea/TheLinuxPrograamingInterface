/**
 * @file simple_tee.c
 * @author Muhammad Tariq (mohamedtareck95@gmail.com)
 * @brief Implement the –a command-line option (tee –a file), which causes tee
            to append text to the end of a file if it already exists. (Refer to Appendix B for a
            description of the getopt() function, which can be used to parse command-line
            options.)
 * 
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUF_SIZ
#define BUF_SIZ 1024
#endif

#ifndef MAX_OUT_FILES
#define MAX_OUT_FILES 128
#endif

typedef enum { FALSE, TRUE} Bool;

void helpAndLeave(const char *progname, int status);
void failure(const char *fCall);

int main(int argc, char **argv)
{
    int fd, opt, flags; 
    int i, numFiles = 0;
    mode_t mode;
    int fds[MAX_OUT_FILES];
    Bool append = FALSE;
    char buf[BUF_SIZ + 1];
    ssize_t numRead;

    // parse options
    while ((opt = getopt(argc, argv, "ah")) != -1) {
        switch (opt) {
            case 'a': append = 1;                         break;
            case 'h':helpAndLeave(argv[0], EXIT_SUCCESS); break;
            default:helpAndLeave(argv[0], EXIT_FAILURE);  break;
            }
        }
    
    // if you just enter only arguments exit.
    if (optind >= argc) {
        helpAndLeave(argv[0], EXIT_FAILURE);
    }

    // stdin re-direction
    flags = O_CREAT | O_WRONLY; // Create file if it doesn't exist, Open for only writing.
    mode =   S_IRUSR | S_IWUSR; // rw- --- ---

    // add append flag to the file flags
    if(append)
    {
        flags |= O_APPEND; // Writes are always appended to end of file.
    }else{
        flags |= O_TRUNC; // Truncate existing file to zero length.
    }

    // parse the command line arguments
    for(i = optind; i < argc ; i++)
    {
        fds[i - optind] = fd = open(argv[i], flags, mode);
        if (fd == -1)
            failure("write");
        
        ++numFiles;
    }
    // Read from the stdin 
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZ)) > 0 )
    {
        if (write(STDOUT_FILENO, buf, numRead) != numRead)
        {
            failure("Write");
        }

        for (i = 0; i < numFiles; i++)
        {
            if (write(fds[i], buf, numRead) != numRead)
            {
                failure("Write");
            }
        }
    }
    if (numRead == -1) {
        failure("read");
    }

    for (i = 0; i < numFiles; ++i) {
        if (close(fds[i]) == -1) {
        failure("close");
        }
    }
    return 0;
}

void helpAndLeave(const char *progname, int status)
{
    fprintf(stderr, "Usage: %s [-a] <file1> <file2> ... <fileN> \n", progname);
    exit(status);
}

void failure(const char *fCall) {
  perror(fCall);
  exit(EXIT_FAILURE);
}
