/*
Sage Jenson and Elizabeth Bentivegna
CSCI 241
Lab09
4 May 2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "master.h"

/*@brief parses input line
*@param line is the input line
*@return an array of strings that is the parsed line
*/
char **parse(char* line){
	
}

/*@brief main loop
*/
void main_loop(){

	int size = LINE_LEN;
	char *line = malloc(size*sizeof(char));
	char** pline;
	//read line of input, do stuff!
	while (fgets(line, size, stdin) != NULL){
		
		//parse the input line
		pline = parse(line);
		
	}
	
	//free line, parsed (char**), 
	free(line);
}

/*@brief main function
*@param argc is argument count
*@param argv is arguments
*@return success or not
*/
int main(int argc, char **argv){
	//loop function
	main_loop();
}
