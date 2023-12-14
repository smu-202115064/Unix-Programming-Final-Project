#ifndef	_BUFFER_H
#define	_BUFFER_H 1

#include <sys/types.h>
#define BUFSIZE 1024

struct buffer {
    char *message;
    size_t length;
};

typedef struct buffer t_buffer;

t_buffer *buffer_create();


#endif
