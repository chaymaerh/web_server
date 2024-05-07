#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "analyste_http.h"


void *threadHandler(void * arg) {
    int s = *((int *)arg);
    gestionClient(s);

   free(arg);
}

int gestionClientThread(int s) {
    int *ads = (int *) malloc(sizeof(int));
    if (ads == NULL) exit(-1);
    *ads = s;
    pthread_t tid;
    pthread_create(&tid, NULL, threadHandler, (void *) ads);
    pthread_detach(tid);
    return 0;
}
