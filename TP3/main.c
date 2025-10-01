#include <fcntl.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <fuse/fuse_lowlevel.h>

#include "my_fuse_lowlevel_ops.h"
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

void my_getattr(fuse_req_t req, fuse_ino_t ino, const char *fi){
    int fd = open(fi, O_RDONLY); 
    struct stat sb; 
    fstat(fd, &sb); 
    void *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0); 
    struct tosfs_inode *fs = (struct tosfs_inode *)mapped; 
    printf("File info:\n"); 
    printf("Inode number: %d\n", fs->inode); 
    printf("Block number for data: %d\n", fs->block_no); 
    printf("User id: %d\n", fs->uid);
    printf("Group id: %d\n", fs->gid);
    printf("Mode: %d\n", fs->mode);
    printf("Permission: %d\n", fs->perm);
    printf("Size: %d\n", fs->size);
    printf("Number of hard link: %d\n", fs->nlink);
    munmap(mapped, sb.st_size); 
    close(fd); 
}

static struct my_fuse_lowlevel_ops hello_ll_oper = {
	.lookup		= NULL,
	.getattr	= my_getattr,
	.readdir	= NULL,
	.read		= NULL,
	.write		= NULL,
    .create     = NULL,
};

int main(int argc, char *argv[])
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_chan *ch;
	char *mountpoint;
	int err = -1;

	if (fuse_parse_cmdline(&args, &mountpoint, NULL, NULL) != -1 &&
	    (ch = fuse_mount(mountpoint, &args)) != NULL) {
		struct fuse_session *se;

		se = fuse_lowlevel_new(&args, &hello_ll_oper,
				       sizeof(hello_ll_oper), NULL);
		if (se != NULL) {
			if (fuse_set_signal_handlers(se) != -1) {
				fuse_session_add_chan(se, ch);
				err = fuse_session_loop(se);
				fuse_remove_signal_handlers(se);
				fuse_session_remove_chan(ch);
			}
			fuse_session_destroy(se);
		}
		fuse_unmount(mountpoint);
	}
	fuse_opt_free_args(&args);

	return err ? 1 : 0;
}