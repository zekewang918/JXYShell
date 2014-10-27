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
#include <limits.h>
#include <sys/wait.h>


#define FAILURE 1
#define SUCCESS 0

#define COMMAND_LENGTH 512
#define MAX_COMMEND 512
#define HISTORY_MAX_SIZE 100

static char* history_list[HISTORY_MAX_SIZE];
static int history_count = 0;

int executeCommand(char* cmd);
char* divideString(char* cmd);
int piping(char* cmd);
void history(char* command);
void printHistory();

struct commend
{
  char cmd[MAX_COMMEND][COMMAND_LENGTH+1];
}commend_line;

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  int stop = 0;

  while(!stop){
    printf("JXYShell$ -");
    cmdLine = readline("> ");
    
    if (!strcmp(cmdLine, "exit")){
      exit(SUCCESS);
    }
    if (cmdLine != NULL){
      history(cmdLine);
      executeCommand(cmdLine);
    }else{
      int rc = fork();
      if (rc < 0){
        fprintf(stderr, "Fork Failed\n");
        exit(FAILURE);
      }else if (rc == 0){
        printf("IM CHILD");
        executeCommand(cmdLine);
      }else{
        int wc = wait(NULL);
        printf("IM DADDY%d", wc);
      }
    }
  }
  return SUCCESS;
}
int executeCommand(char * cmd){
  int num = piping(cmd);
  int i;
  for (i = 0;i<num;i++){
    if (strcmp(commend_line.cmd[i], "history") == 0){
      printHistory();
    }else if (system(commend_line.cmd[i]) == -1){
      return 0;
    }
  }
  return 1;
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


void tabCompletion(){

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
  //printf("%s",command);
  if (history_count < HISTORY_MAX_SIZE){
    //strcpy(history_list[history_count],command);
    history_list[history_count++] = strdup(command);
    //printf("%s",history_list[history_count-1]);
    //history_count++;
  }else{
    free(history_list[0]);
    for (i = 1; i < HISTORY_MAX_SIZE; i++){
      history_list[i-1] = history_list[i];
    }
    strcpy(history_list[HISTORY_MAX_SIZE-1], command);
    history_list[HISTORY_MAX_SIZE-1] = strdup(command);
  }

}
