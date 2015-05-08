/*
Sage Jenson and Elizabeth Bentivegna
CSCI 241
Lab09
4 May 2015
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "master.h"

/*@brief parses input line
*@param line is the input line
*@return an array of strings that is the parsed line
*/
void parse(char *line, char** pline){
	
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

/*@brief fork and create child process
*@param line is the input vector
*@return 1 on success, 0 on failure
*/
int executor(char** line){
	pid_t pid, wpid;
	int child;
	
	pid = fork();
	
	if (pid == 0){
		//child process!
		if (execvp(line[0], line) == -1){
			perror("Problem with child process.");
			exit(EXIT_FAILURE);
		}
	} else {
		//parent process!
		do {
			wpid = waitpid(pid, &child, WUNTRACED);
		} while (!WIFEXITED(child) && !WIFSIGNALED(child));
	}
	return 1;
}

/*@brief handles signals*/
void sig_handler(int sig) {
    signal(sig, sig_handler);
    switch (sig) {
		case SIGINT:
	    	printf("Got a signal, quit sub-processes.");
    }
}

/*@brief main loop
*/
void main_loop(){
	int line_len = LINE_LEN;
	int max_words = MAX_WORDS;
	char *line;
	char** pline;
	
	line = calloc(line_len,sizeof(char));
	pline = calloc(max_words, sizeof(char*));
	
	char* home;
	
	//read line of input, do stuff!
	while (fgets(line, line_len, stdin) != NULL){
		
		//check signals
		signal(SIGINT, sig_handler);
		
		//parse the input line into an array
		parse(line, pline);
		
		//implement built in commands
		if (0==strcmp("exit", *pline)){
			printf("Exiting.");
			break;
		} else if (0==strcmp("myinfo", *pline)){
			printf("My PID is %d and my PPID is %d\n", getpid(), getppid());
		} else if (0==strcmp("cd", *pline)){
			if (NULL == *(pline+1)){ //change working directory to $HOME 
				home = getenv("HOME");
				printf("got home as %s", home);
				if (-1 == chdir(home)){
					perror("This directory doesn't exist.");
					exit(EXIT_FAILURE);
				}
			} else { //change working directory to specified directory
				getcwd(home, LINE_LEN);
				strcat(home, "/");
				strcat(home, *(pline+1));
				if (-1 == chdir(home)){
					perror("This directory doesn't exist.");
					exit(EXIT_FAILURE);
				}
			}
		} else if (NULL != *(pline)){ //execute simple system commands
			executor(pline);
		}
		
	}
	
	//Things to free: line, pline (char**), FINISH THIS SHIT
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
