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

void intr(int code) {
	printf("Handeled signal in sigset ss");
	proceed = 0;
}


void usr2(int code) {
	fprintf(stderr, "\nUSR2\r\n");
	exit(0);
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


int main()
{
	sigset_t sigset;
	struct sigaction usrnewact;
	struct sigaction usr2act;

	sigset_t received_set;

	sigemptyset(&sigset); //Initialise sigset to include all defined functions.
	sigaddset(&sigset, SIGINT);

	usrnewact.sa_mask = sigset; // ignore these signals
	usrnewact.sa_handler = intr; // How to handle signals, through function intr.
	usrnewact.sa_flags = 0;

	proceed = 1;

	/**
	 * int sigaction( int sig, const struct sigaction *act,
                struct sigaction *oact );
                If failed to store sigaction, exit program.
	 *
	 */
	if (sigaction(SIGALRM, &usrnewact, NULL) == -1) exit(0);

	usr2act.sa_handler = usr2;
	usr2act.sa_flags = 0;

	if (sigaction(SIGUSR2, &usr2act, NULL) == -1) exit(0);

	alarm(10);


	/*if (signal(SIGUSR1, sig_handler) == SIG_ERR)
	        printf("\ncan't catch SIGUSR1\n");
	    if (signal(SIGKILL, sig_handler) == SIG_ERR)
	        printf("\ncan't catch SIGKILL\n");
	    if (signal(SIGSTOP, sig_handler) == SIG_ERR)
	        printf("\ncan't catch SIGSTOP\n");
	 */
	int i = 0;
	while(i < 10){
		sleep(2);
		++i;
	}

	if( sigpending(&received_set) == -1) exit(0);

	for(i=0; i < _NSIG-1; ++i)
		if(sigismember(&received_set, i)) fprintf(stderr, "BLOCKED: SIG-%d\n", i );

	/*
	 * Crash in 1 and 2;
	 */

#if 0
	char name[] = "Jhon";
	int age = 10;
	int height = 190;
	int* p = &height;

	// crash line
	//p += 0xEFF;

	printf("Hello, my name is %s.\n", name);
	printf("My age is %d.\n", age);
	printf("I am %d cm tall.\n", *p);
#endif

	return 0;
}
