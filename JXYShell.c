#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>

#define FAILURE 1
#define SUCCESS 0

void executeCommand(char* cmd);

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  int stop = 0;
  while(!stop){
    printf("JXYShell$");
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
  
  char* arg[] = {cmd, NULL};
  execvp(arg[0],arg);
  printf("%s\n", cmd);
  //system(cmd);
}


void piping(){

}

void tabCompletion(){

}

void redirection(){

}

void history(){

}
