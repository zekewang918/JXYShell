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

int executeCommand(char* cmd);
char* divideString(char* cmd);
int piping(char* cmd);

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  int stop = 0;

  while(!stop){
    printf("JXYShell$ -");
    cmdLine = readline("> ");
    printf("123");
    if (!strcmp(cmdLine, "exit")){
      exit(SUCCESS);
    }
    if (cmdLine != NULL){
      executeCommand(cmdLine);
    }else{
      int rc = fork();
      if (rc < 0){
        fprintf(stderr, "Fork Failed\n");
        exit(FAILURE);
      }else if (rc == 0){
        printf("IM CHILD");
        char* arg[] = {strdup("ls"), NULL};
        execvp(arg[0],arg);
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
    if (system(commend_line.cmd[i]) == -1){
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

void history(){

}

/* BACKUP CODE JUST IN CASE
// BACKUP of CORRECT PIPE
      /*char divide[2] = "|";
      char* token;
      token = strtok(cmdLine, divide);
      while(token != NULL){
        system(token);
        token = strtok(NULL, divide);
        
      }*/

