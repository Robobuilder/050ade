#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
//Sig int causes normal termination with : [2]-  Interrupt               ./a.out
//SIGQUIT #0  0x00000000004005e3 in main () at infinite.c:18

/**
 * A function sig_handler is used a s a signal handler.
 * This function is registered to the kernel by pasigseting it as the second argument
 * of the system call ‘signal’ in the main() function.
 * The first argument to the function ‘signal’ is the signal we intend the
 * signal handler to handle which is SIGINT in this case.
 */

int proceed;
void instl(int code) {
	//printf("In the handler");
	proceed = 0;
}


void usr2(int code) {
	fprintf(stderr, "\nUSR2\r\n");
	//exit(0);
}

/*void sig_handler(int signo)
{
	if (signo == SIGUSR1)
	        printf("received SIGUSR1\n");
	    else if (signo == SIGKILL)
	        printf("received SIGKILL\n");
	    else if (signo == SIGSTOP)
	        printf("received SIGSTOP\n");
}*/

void
usr2_handler(void)
{
	struct sigaction usr2act;

	usr2act.sa_handler = usr2;
	usr2act.sa_flags = 0;

	if (sigaction(SIGUSR2, &usr2act, NULL) == -1) exit(0);

}

sigset_t blockmask;
void
install_handler(void)
{
      struct sigaction setup_action;

      sigemptyset (&blockmask); // Block these signals when executing signal handler.
      /* Block other terminal-generated signals while handler runs. */
      sigaddset (&blockmask, SIGINT);
      sigaddset(&blockmask,SIGQUIT);
      sigaddset(&blockmask,SIGUSR1);
      setup_action.sa_handler = instl;
      setup_action.sa_mask = blockmask;
      setup_action.sa_flags = 0;
      if( sigaction (SIGTSTP, &setup_action, NULL) == -1) exit(0);
      if (sigaction(SIGALRM, &setup_action, NULL) == -1) exit(0);
    }


int main()
{
	install_handler();
	usr2_handler();

	sigset_t received_set;
	alarm(20);

	proceed = 1; // Flag

	sigprocmask(SIG_BLOCK, &blockmask, &received_set);

	printf("Entering loop");
	int k = 0;
	while(proceed == 1){
		k ++;
	}
	printf("This is how many times we executed: ");
	printf("%i", k);
	printf("Exiting loop");

	if(sigpending(&received_set) == -1) exit(0);

	int i;
	for(i = 0; i < _NSIG-1; ++i)
		if(sigismember(&received_set, i)) fprintf(stderr, "BLOCKED: SIG-%d\n", i );

	/**
	 *   The signals in set are removed from the current set of blocked
     *   signals.  It is permissible to attempt to unblock a signal which is
     *   not blocked.
	 */

	sigprocmask(SIG_UNBLOCK, &blockmask, &received_set);

	return 0;
}
