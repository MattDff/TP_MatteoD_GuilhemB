#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 8

int main(){
    struct stat sb;
    //char* buffer[];

    int file = open("texte.txt", O_RDWR);
    stat("texte.txt",&sb);
    long size_file = sb.st_size;
    printf("Taille du fichier : %ld octets\n",size_file);

    /*
    for (int octet = 0; octet<size_file;octet += BUFFER_SIZE){
        mmap();
    }
    */
        

    return 0;
}