/**
 * @file     xsh_mutex.c
 * @provides xsh_mutex
 *
 * $Id: xsh_mutex.c 2017-11-17 caevans $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <shell.h>
#include <proc.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Shell command (mutex) creates three tasks each
 *  sleeping for a period of time then editing a 
 *  shared resource using mutex exclusion.
 * @param stdin  descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */
 
// declare global variables for the semaphore and counter
semaphore mutex;
int counter2;

// each function used by the semaphore
void proc4(void) {
    while (1) {

	// sleep for a random amount of time
	sleep(rand()%2000);

        // wait for resource to open
	wait(mutex);

	// increment counter
	counter2++;
	printf("PROC1 adds 1 to counter. New value: %d\n", counter2);
	if ( counter2 > 97 ){
	    printf("\nxsh$ ");
	    return OK;
	}

	// signal editing done
	signal(mutex);
    }
}

void proc5(void) {
    while (1) {
	sleep(rand()%2000);
	wait(mutex);
	counter2 = counter2 + 2;
	printf("PROC2 adds 2 to counter. New value: %d\n", counter2);
	if ( counter2 > 97 ){
	    printf("\nxsh$ ");
	    return OK;
	}
	signal(mutex);
    }
}

void proc6(void) {
    while (1) {
	sleep(rand()%2000);
	wait(mutex);
	counter2 = counter2 + 3;
	printf("PROC3 adds 3 to counter. New value: %d\n", counter2);
	if ( counter2 > 97 ){
	    printf("\nxsh$ ");
	    return OK;
	}
        signal(mutex);
    }
}

command xsh_mutex(ushort stdin, ushort stdout, ushort stderr,
                 ushort nargs, char *args[])
{

	// Output help, if '--help' argument was supplied
	if (nargs == 2 && strncmp(args[1],"--help", 6) == 0) 
	{
		fprintf(stdout, "Displays three tasks sleeping for a random period of time and editing\na shared resource using a semaphore and mutual exclusion.\nType without arguments to use command.\n\n" );
		return OK;
	}

	// Check for correct number of arguments
	if (nargs > 1) 
	{
		fprintf(stderr, "mutex: arguments not accepted\n");
		fprintf(stderr, "Try 'mutex --help' for more information\n\n");
		return SYSERR;
	}

    //initialize variables
    counter2 = 0;
    mutex = newsem(1);

    // create semaphore functions
    ready(create((void*) proc4, INITSTK, INITPRIO, "PROC1", 2, 0, NULL), RESCHED_NO);
    ready(create((void*) proc5, INITSTK, INITPRIO, "PROC2", 2, 0, NULL), RESCHED_NO);
    ready(create((void*) proc6, INITSTK, INITPRIO, "PROC3", 2, 0, NULL), RESCHED_NO);

    return OK;

}
