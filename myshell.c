/*************************************************************************************
 * myshell - student code for Project 1 of CSCE 489 
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shellfuncts.h"
#include <string.h>
#include <sys/wait.h>

//i define MAX_LIMIT to be 100 and use it as various buffer sizes so it is easier to change if needed
#define MAX_LIMIT 100

int main(int argv, const char *argc[]) {
	(void) argv; // Make compile warnings go away - be sure to delete this line if you use the param
	(void) argc; // Make compile warnings go away - be sure to delete this line if you use the param

	//print pid of main process
	printf("Main process id: %d\n", getpid());

	//variable to store user inputted command with a buffer size of 100
	char userInput[MAX_LIMIT];

	//variable to store current directory of the user with a buffer size of 100
	char currentDirectory[MAX_LIMIT];
	
	//get user's current directory and print it out
	//we only need to get current directory once because this program does not support changing directories
	getcwd(currentDirectory, sizeof(currentDirectory));
	printf("%s> ", currentDirectory);

	//get user input (example used from geeksforgeeks.org/gets-is-risky-to-use
	fgets(userInput, MAX_LIMIT, stdin);
        int len = strlen(userInput);

	//takes care of case if user presses enter without any input and case if user inputs spaces
	if(userInput[0] != '\n' && userInput[0] != ' '){
		userInput[len-1] = '\0';
	}


	/*tokenizes string into 5 tokens
	*1st token must be the command
	*2nd token can be file name (only for create, update, and list functions) or & symbol (only for dir)
	*3rd token can be number of times to append a text (only for update function) or & symbol (only for create)
	*4th token is text to append (only for update function)
	*5th token is & symbol (only for update)
	*& symbol after any command (with right spacing) indicates command should be run in background*/
	
	char *command = strtok(userInput, " ");

	//takes care of case if user inputs any space before a command
	if(userInput[0] == ' '){
		command = "INVALID";
	}

	char *fileName = strtok(NULL, " ");
	char *n = strtok(NULL, " ");
	char *text = strtok(NULL, "\"");
	char *amp = strtok(NULL, " ");

	//while command is not halt
	while (strcmp(command, "halt") != 0){

		//if command is create
		if(strcmp(command, "create") == 0){
			//throw error if file name is not specified
			if(fileName == NULL){
				printf("Error: Did not specify file name.\n");
			}

			//n is the parameter after fileName
			//if it is not empty (NULL) and not the & symbol, we throw an error message
			else if(n != NULL && strcmp(n, "&") != 0){
				printf("Error: Too many parameters given. This function takes in one file name as a string.\n");
			}

			//if all is well, fork a child process to carry out create command
			//to get to this else statement, we know either n == null or n == &
			else{
				//this means we're in the child process
				//so we print out the child pid and have it carry out create command
				if(fork() == 0){
					printf("\nChild process id for create: %d\n", getpid());
					create(fileName);
			
				}

				//if n does not equal NULL, it must equal & symbol because of top else statement
				//thus, it implies that if n is null, there was no & symbol given so main process
				//needs to wait for child process to end
				if(n == NULL){
					wait(NULL);
				}

			}
			
		}

		//if command is update
		else if(strcmp(command, "update") == 0){

			//check if all required params are included (short-circuits)
			//throw an error if any required params are missing
			if(fileName == NULL || n == NULL || text == NULL){
				printf("Error: Either did not specify file name, number of times to append, or text to append.\n");
			}
			
			//if & symbol is provided, it must be the 5th param given
			//if the 5th param is not empty (NULL) and does not equal the & symbol
			//throw an error
			else if(amp != NULL && strcmp(amp, "&") != 0){
				printf("Error: Too many parameters given. This function takes in a file name as a string, a number of times to append a string, and the text to append as a string.\n");
			}

			//if params are there, fork a new child process to carry out update command
			//to get to this else statement, we know either amp == null or amp == &
			else{
				
				//this means we're in the child process
				//so we print out the child pid and have it carry out update command
				if(fork() == 0){
					printf("\nChild process id for update: %d\n", getpid());
					if(text[strlen(text)-1] == '&'){
						text[strlen(text)-1] = '\0';
						*amp = '&';
					}
					update(fileName, atoi(n), text);          //original n is part of a string input											//so we convert it to an int
				}
				//if amp does not equal NULL, it must equal & symbol because of the top else statement
				//so if amp is null here, it means no & symbol was given and main process must wait
				//for child process to finish
				if(amp == NULL){
					wait(NULL);
				}
			}
		}

		//if command is list
		else if(strcmp(command, "list") == 0){
			//throw error if file name is not specified
			if(fileName == NULL){
				printf("Error: Did not specify file name.\n");
			}

			//similar to create, the 3rd param should either be empty or the & symbol
			//if it is not empty and not equal to the & symbol, we throw an error
			else if(n != NULL && strcmp(n, "&") != 0){
				printf("Error: Too many parameters given. This function takes in one file name as a string.\n");
			}
			//if file name is specified, fork a child process to carry out create command
			//to get to this else statement, we know either n == null or n == &
			else{
				//this means we're in the child process
				//so we print out the child pid and have it carry out the list command
				if(fork() == 0){
					printf("\nChild process id for list: %d", getpid());
					list(fileName);
				}

				//if n does not equal NULL, it must equal & symbol because of top else statement
				//so if n is null, it means & was not entered and main process needs to wait
				//for child process to finish
				if(n == NULL){
					wait(NULL);
				}
			}	
		}
		//if command is dir
		else if(strcmp(command, "dir") == 0){
			
			//2nd param given should either be empty (NULL) or & symbol
			//if it is not empty and not equal to & symbol, we throw an error
			if(fileName != NULL && strcmp(fileName, "&") != 0){
				printf("Error: Too many parameters given. This function accepts no parameters.\n");
			}
			
			//to get in the else statement, we know either fileName == null or fileName == &
			else{
				//this means we're in the child process
				//so we print out the child pid and have it carry out the dir command
				if(fork() == 0){
					printf("\nChild process id for dir: %d", getpid());
					dir();
				}
				
				//if fileName does not equal NULL, it must equal & because of top if statement
				if(fileName == NULL){
					wait(NULL);
				}
			}

		}

		//if command doesn't exist, have user try again
		else{
			printf("Error: Command not found. Try again.\n");
		}
		
		//after every command or command attempt, we print the directory again and get user input
		printf("\n%s> ", currentDirectory);
		fgets(userInput, MAX_LIMIT, stdin);
        	len = strlen(userInput);

		//same check as before
		//this takes care case where user presses enter without typing anything or if user enters just space characters
		if(userInput[0] != '\n' && userInput[0] != ' '){
			userInput[len-1] = '\0';
		}
		
		//same token splitting as earlier
		command = strtok(userInput, " ");

		//takes care of case where commands are preceded by spaces
		if(userInput[0] == ' '){
			command = "INVALID";
		}

		fileName = strtok(NULL, " ");
		n = strtok(NULL, " ");
		text = strtok(NULL, "\"");
		amp = strtok(NULL, " ");
	}

	//breaks out of the while loop if user types halt
	//and carries out halt command
	halt();
}
