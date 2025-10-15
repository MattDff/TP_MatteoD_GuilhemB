#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segdef.h"

struct sembuf sop;

void init(int shmid, int semid, segment* seg){
    if ((shmid=shmget(cle,segsize,0)) == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if ((semid=semget(cle,3,0)) == -1){
        perror("semget");
        exit(EXIT_FAILURE);
    }

    if ((seg = (segment *) shmat(shmid, NULL, 0)) ==  -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    init_rand();
}

int main(int argc, char *argv[]){
    int shmid, semid;
    segment* seg;
    init(shmid,semid,seg);
    return 0;
}