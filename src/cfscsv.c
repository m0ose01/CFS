#include <cfscsv.h>

int write_csv_int2(CFSFile *file, FILE *csv_file)
{
	const int CHANNEL_COUNT = file->header->general_header->channel_count;
	const int DS_COUNT = file->header->general_header->data_section_count;

	fprintf(csv_file, "channel,data_section,x,y\n");
	for (int current_ds = 0; current_ds < DS_COUNT; current_ds++)
	{
		for (int current_channel = 0; current_channel < CHANNEL_COUNT; current_channel++)
		{
			if (file->data_sections->channel_data[current_channel].data_type != INT2)
			{
				continue;
			}

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
				float current_y = y_scale * (((int16_t *)current_channel_data->data)[current_point] + y_offset);
				fprintf(csv_file, "%i,%i,%f,%f\n", current_channel,current_ds,current_x,current_y);
			}
		}
	}
	return 0;
}
