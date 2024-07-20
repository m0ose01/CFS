cfs: cfs.h cfs.c
	cc -o cfs cfs.c -Wall -Wextra -Wpedantic

debug: cfs.h cfs.c
	cc -o cfs cfs.c -O0 -g -Wall -Wextra -Wpedantic
