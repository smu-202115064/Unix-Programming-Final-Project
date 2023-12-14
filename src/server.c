#include <stdio.h>
#include <stdlib.h>
#include "worker.h"


int main(int argc, char *argv[]) {
    int worker_id = atoi(argv[1]);
    printf("worker id : %d\n", worker_id);
    run_worker(worker_id);
}
