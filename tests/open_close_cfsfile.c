#include <cfs.h>
#include <cfsapi.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <crtdbg.h>
#define DETECT_HEAP_CORRUPTION _CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF)
#else
#define DETECT_HEAP_CORRUPTION (void)0
#endif

int main(int argc, char *argv[])
{
	DETECT_HEAP_CORRUPTION;
	if (argc < 2)
	{
		printf("ERROR: File not supplied as argument.\n");
		return 1;
	}
	char *input_file_name = argv[1];
	cfs_short handle = OpenCFSFile(input_file_name, 0, 1);
	if (handle < 0)
	{
		printf("ERROR: Got invalid file handle %d\n", handle);
		return 3;
	}

	CloseCFSFile(handle);
}
