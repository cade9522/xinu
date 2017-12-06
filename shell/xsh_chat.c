/**
 * @file     xsh_chat.c
 * @provides xsh_chat
 *
 * $Id: xsh_chat.c 2017-11-11 caevans $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <shell.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Shell command (chat) allows for two users to send/recieve
 *  messages across two different routers
 * @param stdin  descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */

//initialize checks for user status
int user1 = 0;
int user2 = 0;

command xsh_chat(ushort stdin, ushort stdout, ushort stderr,
                 ushort nargs, char *args[])
{

	// Output help, if '--help' argument was supplied
	if (nargs == 2 && strncmp(args[1],"--help", 6) == 0) 
	{
		fprintf(stdout, "type chat with no arguments to enter the chat room.\nTo exit the chat, type '#exit'.\n\n" );
		return OK;
	}

	// Check for correct number of arguments
	if (nargs > 1) 
	{
		fprintf(stderr, "chat: arguments unsupprted\n");
		fprintf(stderr, "Try 'chat --help' for more information\n\n");
		return SYSERR;
	}

    // declare string, char counter, and ushort for output
    char text[50];
    int i;
    ushort other;
    
    // set output variable to the opposite UART and user flag to true
    if (stdin == TTY0){
	other = TTY1;
	user1 = 1;
    }
    else {
	other = TTY0;
	user2 = 1;
    }
    
    // print chat message to terminal and other user if  active
    fprintf(stdout, "\033[1;36m\e[3m*You have entered the chat\e[0m\033[0m\n");
    if ( (other == TTY0 && user1 == 1) || (other == TTY1 && user2 == 1) ){
	 write(other, "\033[1;32m\e[3m*Buddy Guy has entered the chat\e[0m\033[0m\n", 51);
    }

    // continuously loop through reads/writes until told to exit
    while(1){

	// clear string
        for (i = 0; i < 50; i++){
	    text[i] = NULL;
	}

	// read input
	read(stdin, text, 50);

	// if #exit, send output message and exit
 	if ( strncmp(text, "#exit", 5) == 0 ){
	    if ( (other == TTY0 && user1 == 1) || (other == TTY1 && user2 == 1) ){
		 write(other, "\033[1;32m\e[3m*Buddy Guy has left the chat\e[0m\033[0m\n", 48);
	    }
	    
	    // reset user flag
	    if ( other == TTY0 ){
		user2 = 0;
	    }
	    else {
		user1 = 0;
	    }
	    fprintf(stdout, "\033[1;36m\e[3m*You have left the chat\e[0m\033[0m\n\n");
	    return OK;
	}

	// print text on terminal
	fprintf(stdout, "\033[1;36mYou: %s\033[0m", text);

	// write text to opposite terminal
	if ( (other == TTY0 && user1 == 1) || (other == TTY1 && user2 == 1) ){
	    write(other, "\033[1;32mBuddy Guy: ", 18);
	    write(other, text, 50);
	    write(other, "\033[0m", 4);
	}
    }
}
