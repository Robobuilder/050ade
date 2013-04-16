#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

    int fd[2];
    int pid;
    char read_buf[argc + 1];
    int chars_read;

    /*
     * Check for adequate args.
     */
    if (argc < 1) {
        fprintf(stderr, "usage: grab-stdout prog arg ...");
        exit(-1);
    }

    /*
     * Create a pipe that will have its output written to by the executed
     * program.
     */
    pipe(fd);

    /*
     * Fork off a process to exec the program.
     */
    if ((pid = fork()) < 0) {
        perror("fork");
  exit(-1);
    }

    /*
     * Forked child has its stdout be the write end of the pipe.
     */
    else if (pid == 0) {

        /*
         * Don't need read end of pipe in child.
         */
        close(fd[0]);

        /*
         * This use of dup2 makes the output end of the pipe be stdout.
         */
        dup2(fd[1], STDOUT_FILENO);

        /*
         * Don't need fd[1] after the dup2.
         */
        close(fd[1]);

        /*
         * Exec the program given in the command line, including any args.
         * The functions execlp and execvp will duplicate the actions of the shell
         *  in searching for an executable file if the specified file name does not
         *   contain a slash (/) character. The search path is the path specified in
         *   the environment by the PATH variable. If this variable isn't specified,
         *   the default path ``:/bin:/usr/bin'' is used. In addition, certain errors
         *    are treated specially.
         */

        execlp("ls", "ls", 0);

        perror("exec1");
        exit(-1);
    }


    else {
    	/*
    	 * Parent takes its input from the read end of the pipe.
    	 */
    	dup2(fd[0], STDIN_FILENO);
        //I'm unsure of what to do here but this is all I could think of
        execlp("sort", "sort", "-r");

        perror("exec2");
        exit(-1);

    }

    exit(0);
}
