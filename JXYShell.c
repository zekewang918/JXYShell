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

void executeCommand(char* cmd);
char* divideString(char* cmd);
int piping(char* cmd);

struct commend
{
  char cmd[MAX_COMMEND][COMMAND_LENGTH+1];
}commend_line;

int 
main(int argc, char *argv[])
{
  char* cmdLine;
  int stop = 2;
  //char buff[PATH_MAX + 1];
  //char* cwd = getcwd(buff, PATH_MAX+  1);
  while(stop){
    printf("JXYShell$ -");
    cmdLine = readline("> ");
    //char* const args[] = {cmdLine,NULL};
    //printf("%s123", cmdLine);
    
    if (!strcmp(cmdLine, "exit")){
      exit(SUCCESS);
    }
    if (system(cmdLine) != -1){
      //system(cmdLine);
    }else{
      int rc = fork();
    if (rc < 0){
      fprintf(stderr, "Fork Failed\n");
      exit(FAILURE);
    }else if (rc == 0){
      printf("IM CHILD");
      //system(cmdLine);
      //char *myargs[3];
      //myargs[0] = strd
      char* arg[] = {strdup("ls"), NULL};
      execvp(arg[0],arg);
      //executeCommand(cmdLine);
    }else{
      int wc = wait(NULL);
      printf("IM DADDY%d", wc);
    }
    stop--;
    }
  }
  return SUCCESS;
}
void executeCommand(char * cmd){
  printf("CMD:%s", cmd);
  int num = piping(cmd);
  printf("NUMBER: %d",num);
  int i;
  for (i = 0;i<num;i++){
    //printf("%s", commend_line.cmd[i]);
    system(commend_line.cmd[i]);
  }
  char* arg[] = {cmd, NULL};
  execvp(arg[0],arg);
  //printf("%s\n", cmd);
  //system(cmd);
  //exit(FAILURE);
}

/*char* divideString(char* cmd){

}*/


int piping(char* cmd){
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
  return index;
}

void tabCompletion(){

}

void redirection(){

}

void history(){

}
