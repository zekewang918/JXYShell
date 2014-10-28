/* * * * * * * * * * * * * * * 
 * JXY SHELL                 *
 *                           *
 * A Simple Shell            *
 *                           *
 * Authors: Jingjing Tang    *
 *          Xike Wang        *
 *          Yang Yang        *
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>


#define FAILURE 1
#define SUCCESS 0

#define COMMAND_LENGTH 16
#define MAX_COMMEND 64
#define HISTORY_MAX_SIZE 100

static char* history_list[HISTORY_MAX_SIZE];
static int history_count = 0;

void executeCommand(int num);
char* divideString(char* cmd);
int piping(char* cmd);
void history(char* command);
void printHistory();
void parse(char* line, char** argc);
void execute(char **argv);




int 
main(void)
{
  char* cmdLine;
  while(1){
    
    printf("JXYShell$ -");
    cmdLine = readline("> ");
    history(cmdLine);
    executeCommand(piping(cmdLine));

  }
  return SUCCESS;
}
struct commend
{
  char cmd[MAX_COMMEND][COMMAND_LENGTH+1];
}commend_line;

void executeCommand(int num){
  int i;
  char *argv[64];
  
  for (i = 0;i<num;i++){
    parse(commend_line.cmd[i], argv);
  }


  int rc = fork();
      if (rc < 0){
        fprintf(stderr, "Fork Failed\n");
        exit(FAILURE);
      }else if (rc == 0){
        execvp(*argv, argv);
      }else{
        wait(NULL);
      }
}

int piping(char* cmd){
  char divide[2] = "|";
  char* token;
  int index = 0;
  token = strtok(cmd, divide);
  while(token != NULL){
    strcpy(commend_line.cmd[index], token);
    index++;
    token = strtok(NULL, divide);
  }
  return index;
}


void  parse(char *line, char **argv)
{
     while (*line != '\0') {       
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     
          *argv++ = line;          
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             
     }
     *argv = (char *) '\0';                
}



void redirection(){

}

void printHistory(){
  int i;
  for (i = 0; i < history_count; i++){
    printf("%d\t%s\n", i, history_list[i]);
  }
}

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
