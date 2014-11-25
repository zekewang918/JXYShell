/*-----------------------------------------------------*
 * JXY SHELL                                           *
 *                                                     *
 * A Simple Shell                                      *
 *                                                     *
 * Authors: Jingjing Tang                              *
 *          Xike Wang                                  *
 *          Yang Yang                                  *
 *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <ctype.h>

/*
 * Define Staus of exit program
 * FAILURE: 1   
 * SUCCESS: 0
 */
#define FAILURE 1
#define SUCCESS 0

/*
 * Define the length of command, max number of command, and 
 * history's size
 * Length of command: 16
 * Max number of command: 64
 * History's size: 100
 */
#define COMMAND_LENGTH 16
#define MAX_COMMAND 64
#define HISTORY_MAX_SIZE 100
#define OUTPUT_SIZE 262144

/*
 * Create a static char array that stores history
 * Create a int that stores the number of history 
 */
static char* history_list[HISTORY_MAX_SIZE];
static int history_count = 0;

/*
 * Define that functions includes in this program
 */
void executeCommand(int num);
int piping(char* cmd);
void history(char* command);
void printHistory();
int parse(char* line, char** argc);
int isBuiltIn(char* cmd);
void trim(char* cmd);

/*
 * Struct a 2D-array that stores the commands
 */
struct command {
  char cmd[MAX_COMMAND][COMMAND_LENGTH+1];
}command_line;


/*
 * Function that execute program by using fork() and execvp()
 */
void executeCommand(int num) {
  FILE *fp;
  int i;
  int j;
  char *argv[64];
  int pipefds[2*num];

  for (j = 0; j < num; j++) {
    if (pipe(pipefds + j * 2) < 0) {
      perror("Pipe Problem");
      exit(FAILURE);
    }
  }

  for (i = 0; i < num; i++) {
    int numOfToken = parse(command_line.cmd[i], argv);
    
    if (*argv[0] == '!') {
      char str[3];
      strcpy(str, *argv);
      if (strlen(str) == 2) {
        int index = str[1]-'0';
        if (index <= history_count)
          *argv = history_list[history_count-index];
      }
    }

    if (strcmp(*argv, "history") == 0)
      printHistory();

    int rc = fork();
      if (rc < 0) {
        fprintf(stderr, "Fork Failed\n");
        exit(FAILURE);
      } else if (rc == 0) {
        if(i != 0) {
          if (dup2(pipefds[2*(i-1)], 0) < 0) {
            perror("Dup Problem !First");
            exit(FAILURE);
          }
        }
        if(i != num - 1) {
          if (dup2(pipefds[2*i+ 1], 1) < 0) {
            perror("Dup Problem !Last");
            exit(FAILURE);
          }
        }

        for (j = 0; j < numOfToken; j++) {
          if (strcmp(argv[j], ">") == 0) {
            argv[j] = NULL;
            fp = fopen(argv[j+1], "w+");
            if (dup2(fileno(fp), 1) < 0) {
              perror("STDOUT WRITE ERROR!");
              exit(FAILURE);
            }
          } else if (strcmp(argv[j], ">>") == 0) {
            argv[j] = NULL;
            fp = fopen(argv[j+1], "a");
            if (dup2(fileno(fp), 1) < 0) {
              perror("STDOUT APPEND ERROR!");
              exit(FAILURE);
            }
          } else if (strcmp(argv[j], "<") == 0) {
            argv[j] = NULL;
            fp = fopen(argv[j+1], "r");
            if (dup2(fileno(fp), 0) < 0) {
              perror("STDIN READ ERROR!");
              exit(FAILURE);
            }
          } else if (strcmp(argv[j], "<<") == 0) {
            argv[j] = NULL;
            fp = fopen(argv[j+1], "r");
            if (dup2(fileno(fp), 0) < 0) {
              perror("STDIN READ ERROR!");
              exit(FAILURE);
            }
          }
        }

        for (j = 0; j < 2*num; j++) {
          close(pipefds[j]);
        }
        execvp(argv[0], argv);
      }
  }

  for (j = 0; j < 2*num; j++) {
    close(pipefds[j]);
  }

  for (j= 0; j < 2*num; j++) {
    wait(0);
  }
}

/*
 * Function that implement piping by identify | signal
 */
int piping(char* cmd) {
  char divide[2] = "|";
  char* token;
  int index = 0;
  token = strtok(cmd, divide);

  while(token != NULL) {
    trim(token);
    strcpy(command_line.cmd[index], token);
    index++;
    token = strtok(NULL, divide);
  }
  return index;
}

/*
 * trim function trims head and tail spaces of each command
 */
void trim(char *cmd) {
  int length = strlen(cmd);
  while (length > 0 && isspace(cmd[length-1])) {
    length--;
  }
  cmd[length] = '\0';

  int headSpace = 0;
  while (cmd[headSpace] != '\0' && isspace(cmd[headSpace])) {
    headSpace++;
  }
  memmove(cmd, cmd,length-headSpace+1);
}

/*
 * Function that does parsing job
 */
int parse(char *line, char **argv) {
  int num = 1;
  while (*line != '\0') {       
    while (*line == ' ' || *line == '\t' || *line == '\n') {
      *line++ = '\0';
      num++;
    } 
    *argv++ = line++;          
    while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
      line++;           
    }
  }
  *argv = (char*) '\0';
  return num;           
}

/*
 * Function that prints the history list
 */
void printHistory() {
  int i;
  for (i = 0; i < history_count; i++) {
    printf("%d\t%s\n", i+1, history_list[i]);
  }
}

/* 
 * Function that stores history of user types
 */
void history(char* command) {
  int i;

  if (history_count < HISTORY_MAX_SIZE) {
    history_list[history_count++] = strdup(command);
  } else {
    free(history_list[0]);
    for (i = 1; i < HISTORY_MAX_SIZE; i++) {
      history_list[i-1] = history_list[i];
    }
    strcpy(history_list[HISTORY_MAX_SIZE-1], command);
    history_list[HISTORY_MAX_SIZE-1] = strdup(command);
  }
}

/*
 * Checks if commands that user types is built in
 * If it does then run it by functions provided in this program
 */
int isBuiltIn(char* cmd) {
  if (strcmp(cmd, "history") == 0) {
    printHistory();
    return 0;
  } else if (strcmp(cmd, "exit") == 0) {
    exit(SUCCESS); 
  }
  return 1;
}

/*
 * Main Function
 * Function that simulate a shell that could do
 * simple jobs
 */
int main(void) {
  char* cmdLine;
  while(1) {
    printf("JXYShell$ -");
    cmdLine = readline("> ");
    if (cmdLine[0] != '!' || strcmp(cmdLine, "!") == 0)
      history(cmdLine);
    if (isBuiltIn(cmdLine)) {
      executeCommand(piping(cmdLine));
    }
  }
  return SUCCESS;
}