/**
 * @file     xsh_semaphore.c
 * @provides xsh_semaphore
 *
 * $Id: xsh_semaphore.c 2017-11-13 caevans $
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
 * Shell command (semaphore) creates three tasks each
 *  sleeping for a period of time then editing a 
 *  shared resource using semaphores.
 * @param stdin  descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */

// declare global variables
semaphore sync1;
semaphore sync2;
semaphore sync3;
int counter;

// functions used for each semaphore
void proc1(void) {
    while(1){

	// wait until OK to use
	wait(sync3);

	// sleep for a random period of time
	sleep(rand()%2000);
	
	// increment counter
	counter++;
	printf("PROC1 adds 1 to counter. New value: %d\n", counter);

	// if counter aboive 100, exit semaphores
	if (counter > 97){
	    printf("\nxsh$ ");
	    return OK;
	}

	// signal next semaphore
	signal(sync1);
    }
}

void proc2(void) {
    while(1){
	sleep(rand()%2000);
	wait(sync1);
	counter = counter + 2;
	printf("PROC2 adds 2 to counter. New value: %d\n", counter);
	if (counter > 97){
	    printf("\nxsh$ ");
	    return OK;
	}
	signal(sync2);
    }
}

void proc3(void) {
    while(1){
	sleep(rand()%2000);
	wait(sync2);
	counter = counter + 3;
	printf("PROC3 adds 3 to counter. New value: %d\n", counter);
	if (counter > 97){
	    printf("\nxsh$ ");
	    return OK;
	}
	signal(sync3);
    }
}

command xsh_semaphore(ushort stdin, ushort stdout, ushort stderr,
                 ushort nargs, char *args[])
{

	// Output help, if '--help' argument was supplied
	if (nargs == 2 && strncmp(args[1],"--help", 6) == 0) 
	{
		fprintf(stdout, "Displays three tasks sleeping for a random period of time and editing\na shared resource using mutlitple semaphores.\nType without arguments to use command.\n\n" );
		return OK;
	}

	// Check for correct number of arguments
	if (nargs > 1) 
	{
		fprintf(stderr, "semaphore: arguments not accepted\n");
		fprintf(stderr, "Try 'semaphore --help' for more information\n\n");
		return SYSERR;
	}

    fprintf(stdout, "\n");

    // initialize variables
    counter = 0;
    sync1 = newsem(0);
    sync2 = newsem(0);
    sync3 = newsem(0);

    // create semaphore functions
    ready(create((void*) proc1, INITSTK, INITPRIO, "PROC1", 2, 0, NULL), RESCHED_NO);
    ready(create((void*) proc2, INITSTK, INITPRIO, "PROC2", 2, 0, NULL), RESCHED_NO);
    ready(create((void*) proc3, INITSTK, INITPRIO, "PROC3", 2, 0, NULL), RESCHED_NO);
    signal(sync3);   
 
    return OK;

}
