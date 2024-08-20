#include <cfs.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("ERROR: File not supplied as argument.\n");
		return 1;
	}
	char *input_file_name = argv[1];
	FILE *input_file = fopen(input_file_name, "r");
	if (input_file == NULL)
	{
		printf("ERROR: File %s does not exist.\n", input_file_name);
		return 2;
	}

	CFSFileGeneralHeader general_header;
	read_file_general_header(input_file, &general_header);

	CFSFileChannelHeader channel_header_1;
	read_file_channel_header(input_file, &channel_header_1);

	if (strncmp(channel_header_1.channel_name, "ECG", strlen("ECG")) != 0)
	{
		printf("ERROR: Channel 0 header name not read correctly.");
		return 3;
	}

	if (strncmp(channel_header_1.y_axis_units, "mV", strlen("mV")) != 0)
	{
		printf("ERROR: Channel 0 y-axis units read incorrectly.");
		return 4;
	}

	if (strncmp(channel_header_1.x_axis_units, "s", strlen("s")) != 0)
	{
		printf("ERROR: Channel 0 x-axis units read incorrectly.");
		return 5;
	}

	CFSFileChannelHeader channel_header_2;
	read_file_channel_header(input_file, &channel_header_2);

	if (strncmp(channel_header_2.channel_name, "Blood Pressure", strlen("Blood Pressure")) != 0)
	{
		printf("ERROR: Channel 1 header name not read correctly.");
		return 3;
	}

	if (strncmp(channel_header_2.y_axis_units, "Pa", strlen("Pa")) != 0)
	{
		printf("ERROR: Channel 1 y-axis units read incorrectly.");
		return 4;
	}

	if (strncmp(channel_header_2.x_axis_units, "s", strlen("s")) != 0)
	{
		printf("ERROR: Channel 1 x-axis units read incorrectly.");
		return 5;
	}

	return 0;
}
