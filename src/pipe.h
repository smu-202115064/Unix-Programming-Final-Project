#ifndef	_SIMPLE_IO_H
#define	_SIMPLE_IO_H 1

#include <stdio.h>
#include <stdlib.h>


void pipe_create(FILE *[2]);
void pipe_readline(FILE *rfp, char *buf, const size_t bufsize);
void pipe_write(FILE *wfp, const char c);

#endif
