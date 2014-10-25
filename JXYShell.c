#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>

#define FAILURE 1
#define SUCCESS 0

#define COMMAND_LENGTH 512
#define MAX_COMMEND 512

void executeCommand(char* cmd);
char* divideString(char* cmd);
struct commend piping(char* cmd);

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  int stop = 0;
  char buff[PATH_MAX + 1];
  char* cwd = getcwd(buff, PATH_MAX+  1);
  while(!stop){
    printf("JXYShell$ %s:", cwd);
    cmdLine = readline("> ");
    int rc = fork();
    if (!strcmp(cmdLine, "exit")){
     exit(SUCCESS);
    }
    if (rc < 0){
      fprintf(stderr, "Fork Failed\n");
      exit(FAILURE);
    }else if (rc == 0){
      executeCommand(cmdLine);
      exit(FAILURE);
    }else{
      wait(NULL);
    }

  }
  return SUCCESS;
}
void executeCommand(char * cmd){
  //char** commend = piping(cmd);
  char* arg[] = {cmd, NULL};
  execvp(arg[0],arg);
  printf("%s\n", cmd);
  //system(cmd);
}

/*char* divideString(char* cmd){

}*/
struct commend
{
  char cmd[MAX_COMMEND][COMMAND_LENGTH+1];
}commend_line;

struct commend piping(char* cmd){
  //char commend[MAX_COMMEND][COMMAND_LENGTH+1];
  char divide[2] = "|";
  char* token;
  int index = 0;
  token = strtok(cmd, divide);
  while(token != NULL){
    strcpy(commend_line.cmd[index], token);
    printf("%s", commend_line.cmd[index]);
    index++;
  }
  return commend_line;
}

void tabCompletion(){

}

void redirection(){

}

void history(){

}
