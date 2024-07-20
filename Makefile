cfs: include/cfs.h src/cfs.c
	cc -o bin/cfs src/*.c -Iinclude -Wall -Wextra -Wpedantic

debug: include/cfs.h src/cfs.c
	cc -o bin/cfs src/*.c -Iinclude -O0 -g -Wall -Wextra -Wpedantic
