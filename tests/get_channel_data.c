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

	CFSChannelData *channel_data_0 = get_channel_data(&cfs_file, 0, 0);

	if (!assert_int_equal(channel_data_0->data_points_count, 256))
	{
		return 3;
	}

	if (!assert_int_equal(channel_data_0->data_type, type_INT2))
	{
		return 4;
	}

	if (!assert_int_equal(((int16_t *)channel_data_0->data)[1], 1024))
	{
		return 5;
	}

	if (!assert_int_equal(((int16_t *)channel_data_0->data)[255], -1024))
	{
		return 5;
	}

	CFSChannelData *channel_data_1 = get_channel_data(&cfs_file, 1, 0);

	if (!assert_int_equal(((int16_t *)channel_data_1->data)[1], 1024))
	{
		return 5;
	}

	if (!assert_int_equal(((int16_t *)channel_data_1->data)[255], -1024))
	{
		return 5;
	}
}
