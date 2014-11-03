/*-----------------------------------------------------*
 * JXY SHELL                                           *
 *                                                     *
 * A Simple Shell                                      *
 *                                                     *
 * Authors: Jingjing Tang                              *
 *          Xike Wang                                  *
 *          Yang Yang                                  *
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>

/*
 * Define Staus of exit program
 * FAILURE: 1   
 * SUCCESS: 0
 */

#define FAILURE 1
#define SUCCESS 0

/*
 * Define the length of command, max number of command, and 
 * history's size
 * Length of command: 16
 * Max number of command: 64
 * History's size: 100
 */


#define COMMAND_LENGTH 16
#define MAX_COMMEND 64
#define HISTORY_MAX_SIZE 100

/*
 * Create a static char array that stores history
 * Create a int that stores the number of history 
 */

static char* history_list[HISTORY_MAX_SIZE];
static int history_count = 0;

/*
 * Define that functions includes in this program
 */

void executeCommand(int num);
int piping(char* cmd);
void history(char* command);
void printHistory();
void parse(char* line, char** argc);
int isBuiltIn(char* cmd);

/*
 * Struct a 2D-array that stores the commands
 */

struct commend
{
  char cmd[MAX_COMMEND][COMMAND_LENGTH+1];
}commend_line;

struct output
{
  char output[MAX_COMMEND][COMMAND_LENGTH+1];
}output_buffer;

/*
 * Function that execute program by using fork() and execvp()
 */

void executeCommand(int num){
  int i;
  char *argv[64];
  //for (i = 0; i< num; i++){
  //printf("%s == ", commend_line.cmd[i]);}
  //printf("%d", num);
  for (i = 0;i<num;i++){
    parse(commend_line.cmd[i], argv);
    //printf("%s - -! ", *argv);
   
  


  int rc = fork();
      if (rc < 0){
        fprintf(stderr, "Fork Failed\n");
        exit(FAILURE);
      }else if (rc == 0){
        execvp(argv[0], argv);
      }else{
        wait(NULL);
      }
  }
}

/*
 * Function that implement piping by identify | signal
 */

int piping(char* cmd){
  char divide[2] = "|";
  char* token;
  int index = 0;
  token = strtok(cmd, divide);

  while(token != NULL){
    //printf("%s---", token);
    strcpy(commend_line.cmd[index], token);
    index++;
    token = strtok(NULL, divide);
  }
  return index;
}

/*
 * Function that does parsing job
 */

void  parse(char *line, char **argv)
{
     while (*line != '\0') {       
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     
            *argv++ = line;          
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
            line++;   
          //printf("%s",*argv);          
     }
     *argv = (char*) '\0';                
}

/*
 * Function that prints the history list
 */

void printHistory(){
  int i;
  for (i = 0; i < history_count; i++){
    printf("%d\t%s\n", i+1, history_list[i]);
  }
}

/* 
 * Function that stores history of user types
 */

void history(char* command){
  int i;

  if (history_count < HISTORY_MAX_SIZE){
    history_list[history_count++] = strdup(command);
  }else{
    free(history_list[0]);
    for (i = 1; i < HISTORY_MAX_SIZE; i++){
      history_list[i-1] = history_list[i];
    }
    strcpy(history_list[HISTORY_MAX_SIZE-1], command);
    history_list[HISTORY_MAX_SIZE-1] = strdup(command);
  }
}

/*
 * Checks if commands that user types is built in
 * If it does then run it by functions provided in this program
 */

int isBuiltIn(char* cmd){
  if (strcmp(cmd, "history") == 0){
    printHistory();
    return 0;
  }else if (strcmp(cmd, "exit") == 0){
    exit(SUCCESS); 
  }
  return 1;
}

/*
 * Main Function
 * Function that simulate a shell that could do
 * simple jobs
 */

int 
main(void)
{
  char* cmdLine;
  while(1){
    
    printf("JXYShell$ -");
    cmdLine = readline("> ");
    history(cmdLine);
    if (isBuiltIn(cmdLine)){
      executeCommand(piping(cmdLine));
    }

  }
  return SUCCESS;
}

