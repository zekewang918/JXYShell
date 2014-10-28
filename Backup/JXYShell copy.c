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
    //scanf("%s", cmdLine);
    //printf("%s",cmdLine);
    //int num = piping(cmdLine);
    //printf("NUM:%d ", num);
    //parse(cmdLine);
    executeCommand(piping(cmdLine));
    //if (!strcmp(cmdLine, "exit")){
    //  exit(SUCCESS);
    //}
    /*if (cmdLine != NULL){
      history(cmdLine);
      executeCommand(cmdLine);
    }else{*/
      
    //}
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
    //printf("%s", commend_line.cmd[i]);
    parse(commend_line.cmd[i], argv);
    //printf("%s",argv[0]);
    //printf("%s",argv[1]);
    /*if (strcmp(commend_line.cmd[i], "history") == 0){
      //printHistory();
    }else if (execvp(*argv, argv) == -1){
    }*/
  }


  int rc = fork();
      //printf("%d", rc);
      if (rc < 0){
        fprintf(stderr, "Fork Failed\n");
        exit(FAILURE);
      }else if (rc == 0){
        //printf("IM CHILD");
        execvp(*argv, argv);
      }else{
        //printf("I WILL WAIT");
        wait(NULL);
        //printf("IM DADDY");
      }
  
  
  
  //char line[COMMAND_LENGTH];

  
}

int piping(char* cmd){
  char divide[2] = "|";
  char* token;
  int index = 0;
  token = strtok(cmd, divide);
  while(token != NULL){
    //printf("%s", token);
    strcpy(commend_line.cmd[index], token);
    index++;
    token = strtok(NULL, divide);
  }
  //strcpy(commend_line.cmd[index], NULL);
  return index;
}


void  parse(char *line, char **argv)
{
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = (char *) '\0';                 /* mark the end of argument list  */
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
