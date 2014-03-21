/*
*  Name: Matt Schreiber
*  Email: schreibm@onid.oregonstate.edu
*  Class: CS311-400
*  Assignment: 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static void
sigusr1_handler(int sig)
{
    char msg[] = "SIGUSR1 has been caught\n";
    if(sig == SIGUSR1)
        write(STDOUT_FILENO, msg, strlen(msg));
}

static void
sigusr2_handler(int sig)
{
    char msg[] = "SIGUSR2 has been caught\n";
    if(sig == SIGUSR2)
        write(STDOUT_FILENO, msg, strlen(msg));
}

static void
sigint_handler(int sig)
{
    char msg[] = "SIGINT has been caught, terminating the program\n";
    if(sig == SIGINT) {
        write(STDOUT_FILENO, msg, strlen(msg));
        exit(EXIT_SUCCESS);
    }
}

void
init_sigaction( struct sigaction *sa,
                int(*sa_siginit)(sigset_t *),
                int sa_flags,
                void(*handler)(int),
                int sig)
{
    sa_siginit(&sa->sa_mask);
    sa->sa_flags = sa_flags;
    sa->sa_handler = handler;
    if(sigaction(sig, sa, NULL) == -1)
        exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    struct sigaction sa_sigusr1, sa_sigusr2, sa_sigint;
    int signums[] = {   SIGUSR1,
                        SIGUSR2,
                        SIGINT  };
    pid_t my_pid = getpid();
    int i;

    init_sigaction(&sa_sigusr1, &sigemptyset, 0, &sigusr1_handler, SIGUSR1);
    init_sigaction(&sa_sigusr2, &sigemptyset, 0, &sigusr2_handler, SIGUSR2);
    init_sigaction(&sa_sigint, &sigemptyset, 0, &sigint_handler, SIGINT);

    for(i = 0; i < 3; i++) {
        kill(my_pid, signums[i]);
        sleep(1);
    }

    /* Shouldn't reach this point,
       but it's here anyway */
    return 0;
}

