bin/cfs: include/cfs.h include/cfscsv.h src/cfs.c src/cfscsv.c
	cc -o bin/cfs src/cfs.c src/cfscsv.c -Iinclude -Wall -Wextra -Wpedantic

debug: include/cfs.h include/cfscsv.h src/cfs.c src/cfscsv.c
	cc -o bin/cfs src/cfs.c src/cfscsv.c -Iinclude -O0 -g -Wall -Wextra -Wpedantic
