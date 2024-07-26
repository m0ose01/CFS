bin/cfstocsv: include/cfs.h include/cfscsv.h src/cfs.c src/cfscsv.c
	cc -o bin/cfstocsv src/cfs.c src/cfscsv.c -Iinclude -Wall -Wextra -Wpedantic

bin/debug/cfstocsv: include/cfs.h include/cfscsv.h src/cfs.c src/cfscsv.c
	cc -o bin/debug/cfstocsv src/cfs.c src/cfscsv.c -Iinclude -O0 -g -Wall -Wextra -Wpedantic
