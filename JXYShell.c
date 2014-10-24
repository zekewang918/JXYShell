#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define failure 1
#define success 0

int 
main(int argc, char *ar[])
{
  char* cmdLine;
  char* path = "/bin/";
  while(1){
    printf("JXYShell: ");
    fgets(buffer, 512, stdin);
    int rc = fork();
    if (rc < 0){
      fprintf(stderr, "Fork Failed\n");
    }else if (rc == 0){
    }else{
      Wait(NULL);
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
