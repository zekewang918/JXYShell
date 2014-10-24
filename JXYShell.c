#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>

#define FAILURE 1
#define SUCCESS 0

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  while(1){
    printf("JXYShell$");
    cmdLine = readline("> ");
    if (!strcmp(cmdLine, "exit")){
      exit(SUCCESS);
    }
    int rc = fork();
    if (rc < 0){
      fprintf(stderr, "Fork Failed\n");
      exit(FAILURE);
    }else if (rc == 0){
    }else{
      wait(NULL);
    }

  }
  return SUCCESS;
}
void piping(){

}

void tabCompletion(){

}

void redirection(){

}

void history(){

}
