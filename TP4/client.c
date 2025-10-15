#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segdef.h"

#define REQ_MAX 3

void init(int* shmid, int* semid, segment** seg){
    if ((*shmid=shmget(cle,segsize,0)) == -1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    printf("%d\n",*shmid);

    if ((*semid=semget(cle,3,0)) == -1){
        perror("semget");
        exit(EXIT_FAILURE);
    }
    printf("%d\n",*semid);

    if ((*seg = (segment *) shmat(*shmid, NULL, 0)) ==  -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    printf("%p\n",*seg);

    init_rand();
}

int main(int argc, char *argv[]){
    int shmid, semid;
    segment* seg;

    init(&shmid,&semid,&seg);
    int cmpt = 0;
    while(cmpt<REQ_MAX){
        acq_sem(semid,seg_dispo);
        long sum = 0;
        for (int j=0; j<maxval; j++){
            seg->tab[j] = getrand()%100;
            sum += seg->tab[j];
        }
        long local_avrg = sum/maxval;
        acq_sem(semid,seg_init);
        wait_sem(semid, res_ok);
        long serv_avrg = seg->result;
        lib_sem(semid,seg_init);
        lib_sem(semid,seg_dispo);

        printf("Client : %d\n",local_avrg);
        printf("Servor : %d\n",serv_avrg);

        cmpt++;
    }

    shmdt(seg);
    return 0;
}