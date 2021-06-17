#pragma once
#include<unistd.h>
#include "file.h"
#ifdef __MINGW32__
#define ENV 1
#else
#define ENV 0
#endif

/**
 * @fn additional command analizer.
 * @brief get analized status and call additional command function
 * @param line command line
 * @return analized status : { @n
 * 0 : input(line) is additional command and called that, @n
 * 1 : command's param missed, @n
 * 2 : command not found
 * 3 : input(line) is not additional command @n
 * -2 : exit @n
 * }
 */
int AnalizeLine(char*);