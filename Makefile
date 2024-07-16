cfs: cfs.h cfs.c
	cc -o cfs cfs.c

cfs_debug: cfs.h cfs.c
	cc -o cfs_debug cfs.c -O0 -g
