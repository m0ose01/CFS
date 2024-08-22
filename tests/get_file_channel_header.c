#include <cfs.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "test.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("ERROR: File not supplied as argument.\n");
		return 1;
	}
	char *input_file_name = argv[1];
	FILE *input_file = fopen(input_file_name, "rb");
	if (input_file == NULL)
	{
		printf("ERROR: File %s does not exist.\n", input_file_name);
		return 2;
	}

	CFSFile cfs_file;
	read_cfs_file(input_file, &cfs_file);

	CFSFileChannelHeader *channel_header_0 = get_file_channel_header(&cfs_file, 0);
	CFSFileChannelHeader *channel_header_1 = get_file_channel_header(&cfs_file, 1);

	if (!assert_str_equal(channel_header_0->channel_name, "ECG"))
	{
		return 3;
	}

	if (!assert_str_equal(channel_header_0->y_axis_units, "mV"))
	{
		return 4;
	}

	if (!assert_str_equal(channel_header_0->x_axis_units, "s"))
	{
		return 5;
	}

	if (!assert_str_equal(channel_header_1->channel_name, "Blood Pressure"))
	{
		return 3;
	}

	if (!assert_str_equal(channel_header_1->y_axis_units, "Pa"))
	{
		printf("ERROR: Channel 1 y-axis units read incorrectly.");
		return 4;
	}

	if (!assert_str_equal(channel_header_1->x_axis_units, "s"))
	{
		return 5;
	}

	return 0;
}
