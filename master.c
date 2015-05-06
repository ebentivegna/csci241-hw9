/*
Sage Jenson and Elizabeth Bentivegna
CSCI 241
Lab09
4 May 2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "master.h"

/*@brief parses input line
*@param line is the input line
*@return an array of strings that is the parsed line
*/
void parse(char *line, char** pline){
	size_t linecap;
	
	char *token;
	token = malloc(LINE_LEN*sizeof(int));
		
	token = strtok(line, DELIMS);
	int ind = 0;
	
	while(token != NULL){
		pline[ind] = strdup(token);
		token = strtok(NULL, DELIMS);
		ind++;
	}
	pline[ind] = NULL;
	free(token);
}

/*@brief main loop
*/
void main_loop(){

	int line_len = LINE_LEN;
	int max_words = MAX_WORDS;
	char *line;
	line = malloc(line_len*sizeof(char));
	*line = (char) NULL;
	char** pline;
	pline = malloc(max_words * sizeof(char*));
	//read line of input, do stuff!
	while (fgets(line, line_len, stdin) != NULL){
		
		//parse the input line into an array
		parse(line, pline);
		
		
		//printf("%s", line);
		int ind = 0;
		char** temp = pline;
		
		while(*temp){
			printf("%s\n", temp[ind]);
			temp++;
		}
		
		
	}
	
	//Things to free: line, pline (char**), 
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
