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
		
	token = strtok(line, DELIMS);
	int ind = 0;
	
	while(token != NULL){
		pline[ind] = strdup(token);
		token = strtok(NULL, DELIMS);
		ind++;
	}
	if (0== strcmp(*(pline+ ind -1),"&")){
	    amped = 1;
	    printf("amped");
	    pline[ind - 1] = NULL;
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
	int test;
	int child = 0;
	int io[2];
	int tempIO[2];
	int fdes;
	io[0] = STDIN_FILENO;
	io[1] = STDOUT_FILENO;
	
	dup2(STDIN_FILENO, tempIO[0]);
	dup2(STDOUT_FILENO, tempIO[1]);

	
	//this is also where we handle input/output redirection!
	char** temp = pline;
	while (NULL != *temp){
		if (strcmp("<", *temp) == 0){
			
			if ( (fdes = open(*(temp+1), O_RDONLY), 0777) != -1){
			    
			    io[0] = fdes;
			    test = dup2( io[0], STDIN_FILENO );

			    if (test < 0){
				perror("dup2 failed");
				exit(EXIT_FAILURE);
			    }

			} else {
				perror("Error opening file.");
				exit(EXIT_FAILURE);
			}
			//so execvp doesn't use extra args
			*temp = NULL;
		} else if (strcmp(">", *temp) == 0){
			if ((fdes = open(*(temp+1), O_WRONLY | O_CREAT), 0777) != -1){
			    io[1] = fdes;
			    fchmod(io[1], S_IRUSR | S_IWUSR | S_IXUSR);
			    
			    test = dup2(io[1], STDOUT_FILENO);
			    if (test < 0){
				perror("dup2 failed");
				exit(EXIT_FAILURE);
			    }
			} else {
				perror("Error opening file.");
				exit(EXIT_FAILURE);
			}
			//so execvp doesn't use extra args
			*temp = NULL;	
		}
		temp++;
	}
	
	
	//printf("IO stuff: %d %d\n", io[0], io[1]);

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
		if  (0 == amped){
		    wpid = waitpid(pid, &child, 0);
		}
	}

	if (STDIN_FILENO != io[0]){
	    close(io[0]);
	    dup2(tempIO[0], STDIN_FILENO);
	    printf("STDIN should be normal again");
	}

	if (STDOUT_FILENO != io[1]){
	    close(io[1]);
	    dup2(tempIO[1], STDOUT_FILENO);
	    printf("STOUT should be normal again.");
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
	
	    //we need to reap children here
	    if (pid == 0){
		printf("I'm about to die");
		kill(pid, SIGKILL);
	    }
	    
	    amped = 0;

	    printf("my shell > ");	
	    fgets(line, line_len, stdin);
	    
	    signal(SIGINT, sig_handler);
    
	    //parse the input line into an array
	    parse(line, pline);
	    
	    /*    
	    int ind = 0;
	    while(NULL != *(pline+ind)){
		printf("%d %s\n", ind, *(pline+ind));
		ind++;
	    }*/

	    //implement built in commands
	    if (0==strcmp("exit", *pline)){
		    printf("%s\n", "Exiting.");
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
	
	int index = 0;
	//Things to free: line, pline (char**), FINISH THIS SHIT
	while(NULL != *(pline+index)){
	    free(*(pline+index));
	    index++;
	}
	free(pline);
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
