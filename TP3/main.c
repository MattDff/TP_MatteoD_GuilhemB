#include <fcntl.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <fuse/fuse_lowlevel.h>

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

#include <fuse/fuse_lowlevel.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

static void my_getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi){
    struct stat st;
    memset(&st, 0, sizeof(st));

    /* Handle the root */
    if (ino == TOSFS_ROOT_INODE) {
        st.st_ino = ino;
        st.st_mode = S_IFDIR | 0755;
        st.st_nlink = 2;
        st.st_uid = getuid();
        st.st_gid = getgid();
        st.st_size = 4096; 
        st.st_mtime = st.st_atime = st.st_ctime = time(NULL);
        fuse_reply_attr(req, &st, 1.0);
        return;
    }

    fuse_reply_err(req, ENOENT);
}




static struct fuse_lowlevel_ops my_ops = {
    .getattr	= my_getattr,
	/*
    .readdir    = my_readdir,
    .lookup     = my_lookup,
    .read       = myread,
    .create     = mycreate,
    .write      = my_write,
    */
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

		se = fuse_lowlevel_new(&args, &my_ops,
				       sizeof(my_ops), NULL);
		if (se != NULL) {
			if (fuse_set_signal_handlers(se) != -1) {
				fuse_session_add_chan(se, ch);
				err = fuse_session_loop(se);
				fuse_remove_signal_handlers(se);
				fuse_session_remove_chan(ch);
			}
			fuse_session_destroy(se);
		}
		fuse_unmount(mountpoint, ch);
	}
	fuse_opt_free_args(&args);

	return err ? 1 : 0;
}