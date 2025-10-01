#include "tosfs.c"
#include "fuse_lowlevel_ops.h"
#include <fuse/fuse_lowlevel.h>

static struct fuse_lowlevel_ops hello_ll_oper = {
	.lookup		= my_lookup,
	.getattr	= my_getattr,
	.readdir	= my_readdir,
	.read		= my_read,
	.write		= my_write,
    .create     = my_create,
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
