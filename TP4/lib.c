#include "/dev/random"
void init_rand(){

}

long segrand(){
    long nb = rand();
    return nb;
}

void wait_sem(long semid, int sem){
    while(1){
        cout << "En attente" << endl;
    }
    return 0;
}

void acq_sem(long semid, int sem){
    
}