/**
 * @file     xsh_schedule.c
 * @provides xsh_schedule
 *
 * $Id: xsh_schedule.c 2017-11-10 caevans $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Shell command (schedule) cycles through one of two 
 *  real-time task schedules until stopped by the user
 * @param stdin  descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */

// task one
void one(ushort stdout) {
    fprintf(stdout, "  Task 1 running\n");
    sleep(1000);
}

// task two
void two(ushort stdout, int time) {
   
    // standard cycle time
    if (time == 10){
        fprintf(stdout, "  Task 2 running\n");
        sleep(1000);
    }
    // if the cycle is a fraction of a standard cycle
    else {
        fprintf(stdout, "  Task 2 running (0.%d)\n", time);
        int rest = time * 100;
        sleep(rest);
    }
}

// task three
void three(ushort stdout) {
    fprintf(stdout, "  Task 3 running\n");
    sleep(1000);
}

// task four
void four(ushort stdout) {
    fprintf(stdout, "  Task 4 running\n");
    sleep(1000);
}

// burn cycle
void burn(ushort stdout, int time) {
    
    // standard cycle
    if (time == 10){
        fprintf(stdout, "  Burn cycle\n");
        sleep(1000);
    }
    // if the cycle is a franction of a standard cycle
    else {
        fprintf(stdout, "  Burn cycle (0.%d)\n", time);
        int rest = time * 100;
        sleep(rest);
    }
}

// shell command
command xsh_schedule(ushort stdin, ushort stdout, ushort stderr,
                 ushort nargs, char *args[])
{

	// Output help, if '--help' argument was supplied
	if (nargs == 2 && strncmp(args[1],"--help", 6) == 0) 
	{
		fprintf(stdout, "Takes in arguments 'one' for the first task, or 'two' for the second.\nReturns the cyclic execution of the selected real-time task.\nType \"ctrl+space then q\" to stop the execution of either.\n\n" );
		return OK;
	}

	// Check for correct number of arguments
	if (nargs < 2) 
	{
		fprintf(stderr, "schedule: missing argument\n");
		fprintf(stderr, "Try 'schedule --help' for more information\n\n");
		return SYSERR;
	}
	if (nargs > 2) 
	{
		fprintf(stderr, "schedule: too many arguments\n");
		fprintf(stderr, "Try 'palindrome --help' for more information\n\n");
		return SYSERR;
	}
   
    // for first task 
    if (strncmp(args[1], "one", 3) == 0){
	
	int frame;
	
	// continously loop through execution
	while(1){
	    fprintf(stdout, "\n\nStarting a new hyperperiod\n");

	    // loop through each frame of the hyperperiod
	    for(frame = 0; frame < 6; frame++){
		fprintf(stdout, "\n Starting a new frame\n");

		switch (frame){
		    case 0:
			one(stdout);
			one(stdout);
			break;
		    case 1:
			two(stdout, 10);
			three(stdout);
			break;
		    case 2:
			one(stdout);
			one(stdout);
			break;
		    case 3:
			two(stdout, 10);
			burn(stdout, 10);
			break;
		    case 4:
			one(stdout);
			one(stdout);
			break;
		    case 5:
			burn(stdout, 10);
			burn(stdout, 10);
			break;
		}
	    }
	}

    return OK;
    }

    // repeat for second task
    else if (strncmp(args[1], "two", 3) == 0){
        
	int frame;
	while(1){
	    fprintf(stdout, "\n\nStarting a new hyperperiod\n");

	    for(frame = 0; frame < 10; frame++){
		fprintf(stdout, "\n Starting a new frame\n");

		switch (frame){
		    case 0:
			one(stdout);
			three(stdout);
			break;
		    case 1:
			two(stdout, 10);
			two(stdout, 8);
			burn(stdout, 2);
			break;
		    case 2:
			one(stdout);
			burn(stdout, 10);
			break;
		    case 3:
			two(stdout, 10);
			two(stdout, 8);
			burn(stdout, 2);
			break;
		    case 4:
			one(stdout);
			burn(stdout, 10);
			break;
		    case 5:
			two(stdout, 10);
			two(stdout, 8);
			burn(stdout, 2);
			break;
		    case 6:
			one(stdout);
			burn(stdout, 10);
			break;
		    case 7:
			four(stdout);
			four(stdout);
			break;
		    case 8:
			one(stdout);
			burn(stdout, 10);
			break;
		    case 9:
			two(stdout, 10);
			two(stdout, 8);
			burn(stdout, 2);
			break;
		}
	    }
	}

    return OK;
    }
    
    // return error if argument not 'one' or 'two'
    else { 
        fprintf(stderr, "invalid argument, type \"schedule one\" or \"schedule two\" only\n\n");
    return SYSERR;
    }

}
