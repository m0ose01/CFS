bin/cfs: include/*.h src/*.c
	cc -o bin/cfs src/*.c -Iinclude -Wall -Wextra -Wpedantic

debug: include/*.h src/*.c
	cc -o bin/cfs src/*.c -Iinclude -O0 -g -Wall -Wextra -Wpedantic
