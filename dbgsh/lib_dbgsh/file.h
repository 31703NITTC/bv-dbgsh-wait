#pragma once
#include<stdio.h>
#include<stdlib.h>
#define BUF_SIZE 256

typedef struct{
  char* str;
  long long len, cur;
}edit_string;

/* read files & split '\\n' to list */
char **ReadFile2Lines(FILE*);
/* free "malloc"ed void* */
void FreeFileLines(char**);

int _INNER_strcmp(const char*, const char*);
int _INNER_strcmp_end(const char*, const char*, char);
int _INNER_strlen(const char*);
int _INNER_strlen_end(const char*, char);

/* initialize typedef struct edit_string */
edit_string *init_edit_string();
/* reinitialize typedef struct edit_string */
edit_string *reinit_edit_string(edit_string*);
/* edit_string.str += charactor */
edit_string *append_edit_string(char, edit_string*);
/* str.pop_back() */
int PB_edit_string(char*, edit_string*);
/* free edit_string */
void free_edit_string(edit_string*);