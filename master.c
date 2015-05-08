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
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>

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
int executor(char** pline){
	pid_t wpid;
	int child;
	int io[2];
	io[0] = STDIN_FILENO;
	io[1] = STDOUT_FILENO;
	

	
	//this is also where we handle input/output redirection!
	char** temp = pline;
	while (NULL != *temp){
		if (strcmp("<", *temp) == 0){
			
			if (-1 != open(*(temp+1), O_RDONLY)){
			    
			    io[0] = open(*(temp+1));


			} else {
				perror("Error opening file.");
				exit(EXIT_FAILURE);
			}
			//so execvp doesn't use extra args
			*temp = NULL;
			
		} else if (strcmp(">", *temp) == 0){
			if (-1 == (io[1] = open(*(temp+1), O_WRONLY | O_CREAT))){
				perror("Error opening file.");
				exit(EXIT_FAILURE);
			} else {
				fchmod(io[1], S_IRUSR | S_IWUSR | S_IXUSR);
			}
			//so execvp doesn't use extra args
			temp = NULL;	
		}
		temp++;
	}
	
	pid = fork();
	
	if (pid < 0){
		perror("Problem with child process.");
		exit(EXIT_FAILURE);
	} else if (pid == 0){
		//child process!
	
		
		if (execvp(pline[0], pline) == -1){
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

void sig_handler(int sigind) {
	
    signal(sigind, sig_handler);
	
    switch (sigind) {
		case SIGINT:
			if (-1 == pid){
				return;
			}
			//kill(pid, SIGINT);
    }
	
	return;
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
	
	pid = -1;
	//read line of input, do stuff!
	
	 while(1) {
		
	    printf("Master > ");	
	    fgets(line, line_len, stdin);
	    
	    signal(SIGINT, sig_handler);
	    //we need to reap children here
	    //TODO
	    
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
	//free(line);
	
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
