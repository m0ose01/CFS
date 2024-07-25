#include <cfscsv.h>

int write_csv(CFSFile *file, FILE *csv_file)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;

	fprintf(csv_file, "channel,data_section,x,y\n");
	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			CFSDataType current_type = file->data_sections->channel_data[current_channel].data_type;

			int idx = current_ds + (current_channel * DS_COUNT);
			CFSDSChannelHeader *current_ds_channel_header = &file->data_sections->header->channel_headers[idx];
			const float y_scale = current_ds_channel_header->y_scale;
			const float y_offset = current_ds_channel_header->y_scale;
			const float x_offset = current_ds_channel_header->x_offset;
			const float x_increment = current_ds_channel_header->x_increment;

			const int POINTS_COUNT = file->data_sections->channel_data[idx].data_points_count; // We must read points from the channel data itself, in case some points were not written successfully.
			CFSChannelData *current_channel_data = &file->data_sections->channel_data[idx];
			for (int current_point = 0; current_point < POINTS_COUNT; current_point++)
			{
				float current_x = (x_offset + current_point) * x_increment;
				char current_y[32];
				switch (current_type)
				{
					case INT2:
						write_int2(current_channel_data->data, current_point, y_scale, y_offset, current_y);
						break;
					case RL8:
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
