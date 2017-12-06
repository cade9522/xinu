/**
 * @file     xsh_palindrome.c
 * @provides xsh_palindrome
 *
 * $Id: xsh_palindrome.c 2017-11-02 caevans $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Shell command (palindrome) determines if a word (string)
 *  is a palindrome or can be rearranged to be a palindrome.
 * @param stdin  descriptor of input device
 * @param stdout descriptor of output device
 * @param stderr descriptor of error device
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */

command xsh_palindrome(ushort stdin, ushort stdout, ushort stderr,
                 ushort nargs, char *args[])
{

	// Output help, if '--help' argument was supplied
	if (nargs == 2 && strncmp(args[1],"--help", 6) == 0) 
	{
		fprintf(stdout, "Returns \"yes\" if word is a palindrome or can be arranged as one.\nReturns \"no\" if word cannot.\n\n" );
		return OK;
	}

	// Check for correct number of arguments
	if (nargs < 2) 
	{
		fprintf(stderr, "palindrome: missing operand\n");
		fprintf(stderr, "Try 'palindrome --help' for more information\n\n");
		return SYSERR;
	}
	if (nargs > 2) 
	{
		fprintf(stderr, "palindrome: too many arguments\n");
		fprintf(stderr, "Try 'palindrome --help' for more information\n\n");
		return SYSERR;
	}

    // initialize letter counter
    int count[26];
    
    // set the count of each letter to '0'
    int loop;
    int loop2 = sizeof(count) / sizeof(count[0]);
    for (loop = 0; loop < loop2; loop++){
        count[loop] = 0;
    }

    char* string = args[1];
    int start = 0;
    int end = strlen(string);

    // for each char in the string, increment associated array position
    while (start < end){
        char c = string[start];
        // if char is not letter return error
        if (c < 65 || (c > 90 && c < 97) || c > 122){
            fprintf(stderr, "word can only contain letters, try again.\n\n");
            return SYSERR;
        }
        else {
            if (c < 90){
                c += 32;
            }
            ++count[c - 97];
        }
        start++;
    }

    // determine if characters can form a palindrome
    int check = 0;
    int i = 0;
    
    // if more than one letter has odd count, string cannot be palindrome
    while ( i < 26 ){
        if ( count[i] % 2 != 0){
            check++;
            if ( check > 1 ){
                fprintf(stdout, "no\n\n");
                return OK;
            }
        }
        i++;
    }

    // return yes
    fprintf(stdout, "yes\n\n");

    return OK;

}
