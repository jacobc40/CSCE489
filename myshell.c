/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shellfuncts.h"
#include <cstring>
#include <sys/wait.h>

int main(int argv, const char *argc[]) {
	(void) argv; // Make compile warnings go away - be sure to delete this line if you use the param
	(void) argc; // Make compile warnings go away - be sure to delete this line if you use the param

	//print pid of main process
	printf("Main process id: %d\n", getpid());

	//variable to store user inputted command with a buffer size of 100
	char userInput[100];

	//variable to store current directory of the user with a buffer size of 100
	char currentDirectory[100];
	
	//get user's current directory and print it out
	getcwd(currentDirectory, sizeof(currentDirectory));
	printf("%s> ", currentDirectory);

	//get user input (example used from geeksforgeeks.org/gets-is-risky-to-use
	fgets(userInput, 100, stdin);
        int len = strlen(userInput);	
	userInput[len-1] = '\0';

	//tokenizes string into 4 tokens
	//1st token is command
	//2nd token is file name (only for create, update, and list functions)
	//3rd token is number of times to append a text (only for update function)
	//4th token is text to append (only for update function)
	char *command = strtok(userInput, " ");
	printf("Command: %s\n", command);
	char *fileName = strtok(NULL, " ");
	printf("File Name: %s\n", fileName);
	char *n = strtok(NULL, " ");
	printf("Number: %s\n", n);
	char *text = strtok(NULL, "\n");
	printf("Text: %s", text);
	//while command is not halt
	while (strcmp(command, "halt") != 0){

		//if command is create
		if(strcmp(command, "create") == 0){
			//throw error if file name is not specified
			if(fileName == NULL){
				printf("Error: Did not specify file name.\n");
				exit(0);
			}

			//if file name is specified, fork a child process to carry out create command
			if(fork() == 0){
				printf("\nChild process id for create: %d\n", getpid());
				create(fileName);
			
			}
			wait(NULL);
			
		}

		//if command is update
		else if(strcmp(command, "update") == 0){

			//check if all required params are included (short-circuits)
			if(fileName == NULL || n == NULL || text == NULL){
				printf("Error: Either did not specify file name, number of times to append, or text to append.\n");
				exit(0);
			}

			//if params are there, fork a new child process to carry out update command
			if(fork() == 0){
				printf("\nChild process id for update: %d\n", getpid());
				update(fileName, atoi(n), text);
			}
			
			wait(NULL);}

		//if command is list
		else if(strcmp(command, "list") == 0){
			//throw error if file name is not specified
			if(fileName == NULL){
				printf("Error: Did not specify file name.\n");
				exit(0);
			}
			//if file name is specified, fork a child process to carry out create command
			if(fork() == 0){
				printf("\nChild process id for list: %d", getpid());
				list(fileName);
			}
			wait(NULL);	
		}
		//if command is dir
		else if(strcmp(command, "dir") == 0){
			//fork a child process to carry out dir command
			if(fork() == 0){
				printf("\nChild process id for dir: %d", getpid());
				dir();
			}
			wait(NULL);

		}

		//if command doesn't exist, have user try again
		else{
			printf("Error: Command not found. Try again.\n");
		}

		printf("\n%s> ", currentDirectory);
		fgets(userInput, 100, stdin);
        	len = strlen(userInput);	
		userInput[len-1] = '\0';

		command = strtok(userInput, " ");
		printf("Command: %s\n", command);
		fileName = strtok(NULL, " ");
		printf("File Name: %s\n", fileName);
		n = strtok(NULL, " ");
		printf("Number: %s\n", n);
		text = strtok(NULL, "\n");
		printf("Text: %s", text);
	}

	//breaks out of the while loop if user types halt
	//and carries out halt command
	printf("Going into halt now...\n");
	halt();
}
