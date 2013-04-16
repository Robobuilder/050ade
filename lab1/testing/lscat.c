
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sighandler(int id)
{
    printf("Got a signal\n");
}

int main(int argc, char *argv[])
{
    /* do a ls | cat */
    char *ls = "ls";
    char *cat = "cat";
    char *lsargs[] = {"ls", NULL };
    char *catargs[] = {"cat", NULL };

    struct sigaction asignal;
    sigemptyset(&asignal.sa_mask);
    asignal.sa_handler = sighandler;
    asignal.sa_flags = 0;
    sigaction(SIGCHLD, &asignal, (struct sigaction *) NULL);

    int status;
    int pid = fork();
    if(pid > 0)
    {
        /* parent waits for SIGCHLD */
        waitpid(-1, &status, 0);
        printf("children finished\n");
    }
    if(pid == 0)
    {
        /* this should spawn an ls. redirect output to cat */
        int fd[2];
        pipe(fd);
        int cid = fork();
        if(cid > 0)
        {
            /* this is the ls */
            close(fd[0]);           /* close read pipe */
            close(STDOUT_FILENO);   /* close standard out */
            dup(fd[1]);             /* make write pipe stand ard out */
            close(fd[1]);           /* close my ptr to write pipe */

           /* The execv and execvp functions provide an array of pointers to null-terminated
            * strings that represent the argument list available to the new program. The first
            *  argument, by convention, should point to the file name associated with the file
            *   being executed. The array of pointers must be terminated by a NULL pointer. */
            execve(ls, lsargs, 0);
            exit(0);
        }
        else
        {
            /* this is the cat */
            close(fd[1]);           /* close write pipe */
            close(STDIN_FILENO);    /* close standard in */
            dup(fd[0]);             /* make read pipe standard in */
            close(fd[0]);           /* close my ptr to read pipe */
            execve(cat, catargs, 0);
            exit(0);
        }
    }
    printf("completed.\n");
}
