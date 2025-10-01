#include <fcntl.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 

#include "tosfs.h"

void showFileSystemInfos(const char *filename) { 
    int fd = open(filename, O_RDONLY); 
    struct stat sb; 
    fstat(fd, &sb); 
    void *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0); 
    struct tosfs_superblock *fs = (struct tosfs_superblock *)mapped; 
    printf("Filesystem info:\n"); 
    printf("Magic number: %d\n", fs->magic); 
    printf("Total blocks: %d\n", fs->blocks); 
    printf("Total inodes: %d\n", fs->inodes);
    printf("Block size: %d\n", fs->block_size);
    printf("Inode root: %d\n", fs->root_inode);
    munmap(mapped, sb.st_size); 
    close(fd); 
}

