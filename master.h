/*
Sage Jenson and Elizabeth Bentivegna
CSCI 241
Lab09
4 May 2015
*/

#ifndef MAIN_H
#define MAIN_H

//constants
#define LINE_LEN 1024
#define MAX_WORDS 100
#define COMMAND_SEGS 10
#define DELIMS " \t\n"

//vars used to send signals
pid_t pid;
int amped;
//functions
//runs the main loop
void main_loop();
//parses input into array
void parse(char *line,char** pline);
//run simple child processes
int executor(char** line);
//handles signals
void sig_handler(int sig);
#endif //MAIN_H
