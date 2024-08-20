#include <stdlib.h>

#include <cfstocsv.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: cfs.c <filename> <output>\n");
		return 1;
	}
	if (argc > 3)
	{
		printf("Usage: cfs.c <filename> <output>\n");
	}
	char *input_file_name = argv[1];
	FILE *cfs_file = fopen(input_file_name, "rb");
	if (cfs_file == NULL)
	{
		printf("ERROR: Failed to open file. Check whether filename is correct.\n");
		return 1;
	}

	CFSFile *file = malloc(sizeof(CFSFile));
	if (file == NULL)
	{
		return 1;
	}
	int success = read_cfs_file(cfs_file, file);
	if (success != 0)
	{
		printf("Failed to read CFS file.\n");
		return 1;
	}

	FILE *csv_file = stdout;
	if (argc == 3)
	{
		csv_file = fopen(argv[2], "w");
		if (csv_file == NULL)
		{
			printf("ERROR: Failed to open CSV file for writing.\n");
			fclose(cfs_file);
			free_cfs_file(file);
			free(file);
			return 1;
		}
	}
	write_csv(file, csv_file);
	fclose(csv_file);

	free_cfs_file(file);
	free(file);
}

int write_csv(CFSFile *file, FILE *csv_file)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;

	fprintf(csv_file, "channel,data_section,x,y\n");
	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			CFSChannelData *current_channel_data = get_channel_data(file, current_channel, current_ds);
			CFSDSChannelHeader *current_ds_channel_header = get_ds_channel_header(file, current_channel, current_ds);
			CFSDataType current_type = current_channel_data->data_type;

			const float y_scale = current_ds_channel_header->y_scale;
			const float y_offset = current_ds_channel_header->y_offset;
			const float x_offset = current_ds_channel_header->x_offset;
			const float x_increment = current_ds_channel_header->x_increment;

			const int POINTS_COUNT = current_channel_data->data_points_count; // We must read points from the channel data itself, in case some points were not written successfully.
			for (int current_point = 0; current_point < POINTS_COUNT; current_point++)
			{
				float current_x = (x_offset + current_point) * x_increment;
				char current_y[32];
				switch (current_type)
				{
					case type_INT2:
						write_int2(current_channel_data->data, current_point, y_scale, y_offset, current_y);
						break;
					case type_RL8:
						write_rl8(current_channel_data->data, current_point, y_scale, y_offset, current_y);
					default:
						continue;
				}
				fprintf(csv_file, "%i,%i,%f,%s\n", current_channel,current_ds,current_x,current_y);
			}
		}
	}
	return 0;
}

int write_int2(void *data, int point, float y_scale, float y_offset, char *buffer)
{
	float y = ((int16_t *)data)[point];
	float y_scaled = (y + y_offset) * y_scale;
	return sprintf(buffer, "%f", y_scaled);
}

int write_rl8(void *data, int point, float y_scale, float y_offset, char *buffer)
{
	double y = ((double *)data)[point];
	double y_scaled = (y + y_offset) * y_scale;
	return sprintf(buffer, "%f", y_scaled);
}
