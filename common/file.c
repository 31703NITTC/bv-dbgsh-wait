#include "file.h"

char **ReadFile2Lines(FILE *fp){
	char **lines = (char**)malloc(sizeof(char*) * BUF_SIZE);
	int lines_len = BUF_SIZE, lines_cur=0;
	char *line = (char*)malloc(sizeof(char) * BUF_SIZE);
	int line_len = BUF_SIZE, line_cur=0;
	char c;

	if(fp == NULL){
		lines[0] = "";
		lines[1] = NULL;
		free(line);
		return lines;
	}
	while((c = fgetc(fp)) != EOF){
		if(c == '\n'){
			line[line_cur++] = '\0';
			lines[lines_cur++] = line;
			if(lines_cur >= lines_len){
				lines = (char**)realloc(lines, lines_len += BUF_SIZE);
			}
			line = (char*)malloc(sizeof(char) * BUF_SIZE);
			line_len = BUF_SIZE;
			line_cur = 0;
		}else{
			line[line_cur++] = c;
			if(line_cur >= line_len){
				line = (char*)realloc(line, line_len += BUF_SIZE);
			}
		}
	}
	line[line_cur++] = '\0';
	lines[lines_cur++] = line;
	if(lines_cur >= lines_len){
		lines = (char**)realloc(lines, lines_len += BUF_SIZE);
	}
	lines[lines_cur++] = NULL;

	return lines;
}

void FreeFileLines(char** lines){
for(int i=0; lines[i]!=NULL; i++){
		free(lines[i]);
	}
	free(lines);
}
/** caution! end charactor missing brake program */
int _INNER_strcmp_end(const char* a, const char* b, char end){
	int i;
	for(i=0; a[i]!=end && b[i]!=end && a[i]!='\0' && b[i]!='\0'; i++){
		if(a[i] - b[i]) return a[i] - b[i];
	}
	if(abs((int)a[i] - (int)b[i]) == end - '\0') return 0;
	return a[i] - b[i];
}
int _INNER_strcmp(const char* a, const char* b){
	return _INNER_strcmp_end(a,b,'\0');
}

int _INNER_strlen_end(const char *s, char end){
	int i = 0;
	while(s[i] != end) i++;
	return i;
}
int _INNER_strlen(const char *s){
	return _INNER_strlen_end(s, '\0');
}

edit_string *init_edit_string(){
	edit_string *e = (edit_string*)malloc(sizeof(edit_string));
	e->str = (char*)malloc(sizeof(char) * BUF_SIZE);
	e->len = BUF_SIZE;
	e->cur = 0;
	return e;
}

edit_string *reinit_edit_string(edit_string *e){
	e->str = (char*)realloc(e->str, sizeof(char) * BUF_SIZE);
	e->len = BUF_SIZE;
	e->cur = 0;
	return e;
}

edit_string *append_edit_string(char c, edit_string *e){
	e->str[e->cur++] = c;
	if(e->cur >= e->len){
		e->str = (char*)realloc(e->str, sizeof(char) * (e->len += BUF_SIZE));
	}
	e->str[e->cur] = '\0';
	return e;
}

int PB_edit_string(char *ret, edit_string *e){
	if(e->cur==0) return 0;
	int r=e->cur;
	*ret = e->str[--e->cur];
	e->str[e->cur] = '\0';
	return r;
}

void free_edit_string(edit_string *e){
	free(e->str);
	free(e);
}