#include "lineAnalizer.h"

int wait_SC(char * args){
  int wait_time = 0;
  for(int i=0; args[i] != '\0' && args[i] != '\n'; i++){
    if(args[i] == ' '){
      continue;
    }else if(args[i] < '0' || '9' < args[i] ){
      printf("\033[31merror: wait args missed. must be integer. (args = %s) look : %d\n\033[0m", args, args[i]);
      return 1;
    }
    wait_time = wait_time * 10 + args[i] - '0';
  }
  if(wait_time >= 600){
    printf("\033[33mWARNING: wait time is large.\n\033[0m");
  }
  sleep(wait_time);
  return 0;
}
int file_SC(char* args){
  printf(" %s ", args);
  FILE *fp = fopen(args, "r");
  if(fp == NULL){
    printf("\033[31mERROR: file cannot open.\n\033[0m");
    return 1;
  }
  char **lines = ReadFile2Lines(fp);
  fclose(fp);
  for(int i=0; lines[i] != NULL; i++) AnalizeLine(lines[i]);
  return 0;
}
int run_SC(char* args){
  fflush(stdin);
  printf("\033[34m > %s \n\033[0m", args);
  if(ENV) system(args);
  return 0;
}
int exit_SC(char* args){
  printf("\033[34m exit(0) \n\033[0m");
  return -2;
}

/** sub command must return integer @n
 * 
 */
int (*com[])(char*) = {wait_SC, file_SC, run_SC, exit_SC};

int AnalizeLine(char* line){
  /* "sh" = "shell" */
  char *COMMAND_NAME = "sh ";
  char *SUBCOMMAND_NAME[] = {"wait ", "file ", "run ", "exit", NULL};

  char *p = line;
  printf("\033[36m%s ... \033[0m", line);
  for(; *p==' ' && *p!='\0'; p++);
  if(_INNER_strcmp_end(COMMAND_NAME, line, ' ')){
    printf("\033[36mthrow BV(3)\n\033[0m");
    return 3;
  }
  p += _INNER_strlen(COMMAND_NAME);
  for(; *p==' ' && *p!='\0'; p++);
  for(int i=0; SUBCOMMAND_NAME[i] != NULL; i++){
    if(!_INNER_strcmp_end(p, SUBCOMMAND_NAME[i], ' ')){
      p += _INNER_strlen(SUBCOMMAND_NAME[i]);
      int r = com[i](p);
      printf("\033[36mdone(%d)\n\033[0m",r);
      return r;
    }
  }
  printf("\033[36mnot found(2)\n\033[0m");
  return 2;
}