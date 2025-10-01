#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef uint64_t fuse_ino_t;
typedef struct fuse_req *fuse_req_t;

struct fuse_lowlevel_ops {
   //void (*init) (void *userdata, struct fuse_conn_info *conn);


   void (*lookup) (fuse_req_t req, fuse_ino_t parent, const char *name);


   void (*getattr) (fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);


   void (*readdir) (fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);


   void (*read) (fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);


   void (*write) (fuse_req_t req, fuse_ino_t ino, const char *buf, size_t size, off_t off, struct fuse_file_info *fi);


   void (*create) (fuse_req_t req, fuse_ino_t parent, const char *name, mode_t mode, struct fuse_file_info *fi);
};
