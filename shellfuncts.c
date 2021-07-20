/*************************************************************************************
 * shellfuncts.c - code definitions for your functions
 *
 *************************************************************************************/

#include <stdio.h>
#include "shellfuncts.h"
#include <unistd.h>
//#include <cstdlib>
#include <stdlib.h>
//#include <cstring>
#include <string.h>


/*************************************************************************************
 * create - creates a file given a file name. Throws an error if file already exists.
 *
 *		Params:	fileName - name of the file in string format
 *
 *		Returns: currently doesn't return anything
 *
 *************************************************************************************/
void create(char *fileName){

	//Create a FILE pointer and attempt to open a file for reading with fileName
	FILE *file;
    file = fopen(fileName, "r");

	//if file points to an actual file instead of null, it means the file already exists and print an error msg
	if(file != NULL){
		printf("Error: The file already exists.\n");
	}

	//if not, create the new file
	else{
		file = fopen(fileName, "w+");	
	}

	//regardless of the above, close file and exit
	fclose(file);
	exit(0);
	
}

/*************************************************************************************
 * update - updates a file given the file name (creates the file if it doesn't already
 * exist), number of times to append a text, and the text to append. Sleeps after
 * appending every line.
 *
 *		Params:	fileName - name of the file in string format
				n - number of times to append in integer format
				text - the string to append to the file
 *
 *		Returns: currently doesn't return anything
 *
 *************************************************************************************/

void update(char *fileName, int n, char *text){
	//Create a FILE pointer and attempt to open a file for reading with fileName
    FILE *file;
	
	//open the file in appending mode
	file = fopen(fileName, "a");

	//write text to file n times
	for(int i = 0; i < n; i++){
		fprintf(file, "%s\n", text);
		sleep(strlen(text)/5);
	}

	printf("The update has completed for process id: %d.", getpid());

	//force file write to disk, close file, and exit
	//question: I read fclose() automatically flushes the buffer...is it redundant to call fflush?
	fflush(file);
	fclose(file);
	exit(0);
	
}

/*************************************************************************************
 * list - prints out the contents of a file given a file name. Throws an error if 
 * file does not exist.
 *
 *		Params:	fileName - name of the file in string format
 *
 *		Returns: currently doesn't return anything
 *
 *************************************************************************************/

void list(char *fileName){
    //Create a FILE pointer and attempt to open a file for reading with fileName
	FILE *file;
    	file = fopen(fileName, "r");
	//if file is null, the file does not exist and error msg will be printed
	if(file == NULL){
		printf("Error: The file does not exist.\n");
		exit(0);
	}

	//if it does exist, print out the file contents and close when done
	else{
		
		//executes bin file cat to print out contents of fileName	
		execl("/bin/cat", "cat", fileName, NULL);

		//close file only if file was opened and exit
		fclose(file);
		exit(0);
	}

}

/*************************************************************************************
 * dir - lists (no relation to above function with the same name) the files in the
 * current directory.
 *
 *		Params:	none
 *
 *		Returns: currently doesn't return anything
 *
 *************************************************************************************/

void dir(){
	//executes bin file of ls in root directory
	execl("/bin/ls", "ls", NULL);
	exit(0);
}

/*************************************************************************************
 * halt - exits out of the shell program
 *
 *		Params:	none
 *
 *		Returns: currently doesn't return anything
 *
 *************************************************************************************/

void halt(){
	exit(0);
}



