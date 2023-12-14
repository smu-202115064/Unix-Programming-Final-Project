#include <stdlib.h>
#include "buffer.h"


t_buffer *buffer_create() {
    return (t_buffer *) calloc(BUFSIZE, sizeof(char));
}
