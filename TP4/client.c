#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#include "segdef.h"

struct sembuf sop;

void init(int shmid, int semid, segment* seg){
    

    if ((shmid=shmget(cle,segsize,0))){
        perror("shmget");
    }

    if ((semid=semget(cle,3,0))){
        perror("semget");
    }

    init_rand();
}

int main(int argc, char *argv[]){
    int shmid, semid;
    segment* seg;
    struct shmseg seg;
    init(shmid,semid,seg);
    return 0;
}