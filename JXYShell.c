#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <system.h>

#define failure 1
#define success 0

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  while(1){
    printf("JXYShell$ ");
    cmdLine = readline("%");
    int rc = fork();
    if (rc < 0){
      fprintf(stderr, "Fork Failed\n");
    }else if (rc == 0){
    }else{
      wait(NULL);
    }

  }
  return 0;
}
void piping(){

}

void tabCompletion(){

}

void redirection(){

}

void history(){

}
