/*
 * Copyright (c) 2007, 2008 University of Tsukuba
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Tsukuba nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __MINGW32__
#include <conio.h>
#define NOECHO()
#define GETCHAR() getch ()
#define PUTCHAR(c) putch (c)
#define ECHO()
#else
#define NOECHO() system ("stty -echo -icanon")
#define GETCHAR() getchar ()
#define PUTCHAR(c) putchar (c), fflush (stdout)
#define ECHO() system ("stty echo icanon")
#endif

#include "lib_dbgsh/file.h"
#include "lib_dbgsh/lineAnalizer.h"

void
e (void)
{
	ECHO ();
}

int
main (int argc, char **argv)
{
	int s, r;
	FILE *fp;
	FILE *infp;

	if (argc >= 2) {
		infp = fopen (argv[1], "r");
	} else {
		infp = NULL;
	}

	fp = NULL; // 
	char **lines = ReadFile2Lines(infp);
	int lines_ptr=0, line_ptr=0;
	/**
	 * return value of function "AnalizeLine"
	 *  0 : called command done
	 *  1 : called command missed
	 *  2 : throw command
	 * -1 : building command
	 * -2 : press ^C
	 */
	int ALret = -1;
	/**
	 * file or stdin
	 * 1 : file
	 * 0 : stdin
	 */
	int fmode = 1;
	fclose(infp);

	edit_string *line = init_edit_string();
	char _c;
	int ESline = 0;

	//vmcall_dbgsh (-1);
	atexit (e);
	NOECHO ();
	s = -1;
	for (;;) {
		r = 0;
		// r = vmcall_dbgsh(s);
		if (r == (0x100 | '\n')) {
			//vmcall_dbgsh (0);
			printf("\033[36mcall exit\n\033[0m");
			exit (0);
		}
		s = -1;
		if (r == 0) {
			//add code
			if(fmode){
				if(lines[lines_ptr] != NULL){
					if(ALret==-1){
						ALret = AnalizeLine(lines[lines_ptr]);
					}
					if(ALret == 3 && lines[lines_ptr][line_ptr] != '\0'){
						// throw command to bitvisor
						s = lines[lines_ptr][line_ptr++];
					}else if(ALret != -2){
						// line fin
						ALret = -1;
						line_ptr = 0;
						lines_ptr++;
						s = '\n';
					}else{
						s = 3;
					}
				}else{
					printf("\033[36mfile end\n\033[0m");
					fflush(stdin);
					fmode = 0;
				}
			}
			if(!fmode){
				if(ALret == -1) printf("> ");
				while(ALret == -1){
					s = GETCHAR ();
					if(s == 3){
						printf("^C\n");
						ALret = -2;
					}else if(s == '\0' || s == -32){
						// Nothing
					}else if(s == '\b'){
						if(line->cur > 0){
							printf("\033[1D\033[0K");
							PB_edit_string(&_c, line);
						}
					}else if(s == '\t'){
						PUTCHAR(s);
						while(line->cur%8!=0) append_edit_string(' ', line);
					}else if(s=='\n' || s=='\r'){
						PUTCHAR('\n');
						ALret = AnalizeLine(line->str);
					}else{
						PUTCHAR(s);
						append_edit_string(s, line);
					}
				}
				if(ALret == 3 && line->str[ESline] != '\0'){
					s = line->str[ESline++];
				}else if(ALret != -2){
					ALret = -1;
					ESline = 0;
					s = '\n';
					reinit_edit_string(line);
				}else{
					s = 3;
				}
			}
			if (s == 3){ // 本番ではなし(bitvisorが終了を伝える)
				return 0;
			}
			// add end
			if (s == 0)
				s |= 0x100;
		} else if (r > 0) {
			r &= 0xFF;
			if (fp) {
				fprintf (fp, "%c", r);
				fflush (fp);
			}
			PUTCHAR (r);
			s = 0;
		}
	}
}