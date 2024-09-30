#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cfsapi.h>
#include <cfs.h>

#define MAX_CFS_FILES 32

int cfs_files_count = 0;
CFSFile *cfs_files[MAX_CFS_FILES];

cfs_short OpenCFSFile(TpStr fName, cfs_short enableWr, cfs_short memTable)
{
	(void) memTable; // Avoids compiler warning. memTable always used in this implementation, as memory usage
	// is not a problem today, but want to preserve public API.
	
	if (enableWr != 0) // Only implementing readonly functions.
	{
		return -99;
	}
	FILE *file = fopen(fName, "rb");
	if (file == NULL)
	{
		return -12;
	}
	cfs_files_count++;
	if (cfs_files_count > MAX_CFS_FILES)
	{
		return -8;
	}
	cfs_short handle = cfs_files_count;
	CFSFile *cfs_file = malloc(sizeof(CFSFile));
	if (cfs_file == NULL)
	{
		return -8;
	}

	int success = read_cfs_file(file, cfs_file);
	if (success != 0)
	{
		return -8;
	}

	fclose(file);

	cfs_files[handle] = cfs_file;
	return handle;
}

cfs_short CloseCFSFile(cfs_short handle)
{
	CFSFile *current_cfs_file = cfs_files[handle];
	free_cfs_file(current_cfs_file);
	free(current_cfs_file);
	return 0;
}

void GetGenInfo(cfs_short handle, TpStr time, TpStr date, TpStr comment)
{
	CFSFile *current_cfs_file = cfs_files[handle];
	memcpy(time, &current_cfs_file->header->general_header->file_creation_time, sizeof(current_cfs_file->header->general_header->file_creation_time));
	memcpy(date, &current_cfs_file->header->general_header->file_creation_date, sizeof(current_cfs_file->header->general_header->file_creation_date));
	memcpy(comment, &current_cfs_file->header->general_header->file_comment, sizeof(current_cfs_file->header->general_header->file_comment));
}

void GetFileInfo(cfs_short handle, TpShort channels, TpShort fileVars, TpShort DSVars, TpUShort dataSects)
{
	CFSFile *current_cfs_file = cfs_files[handle];
	memcpy(channels, &current_cfs_file->header->general_header->channel_count, sizeof(current_cfs_file->header->general_header->channel_count));
	memcpy(fileVars, &current_cfs_file->header->general_header->file_variable_count, sizeof(current_cfs_file->header->general_header->file_variable_count));
	memcpy(DSVars, &current_cfs_file->header->general_header->data_section_variable_count, sizeof(current_cfs_file->header->general_header->data_section_variable_count));
	memcpy(dataSects, &current_cfs_file->header->general_header->data_section_count, sizeof(current_cfs_file->header->general_header->data_section_count));
}

void GetFileChan(cfs_short handle, cfs_short channel, TpStr chanName, TpStr yUnits, TpStr xUnits, TpDType dataType, TpDKind dataKind, TpShort spacing, TpShort other)
{
	CFSFile *current_cfs_file = cfs_files[handle];

	CFSFileChannelHeader *current_file_channel_header = get_file_channel_header(current_cfs_file, channel);
	memcpy(chanName, &current_file_channel_header->channel_name, sizeof(current_file_channel_header->channel_name));
	memcpy(yUnits, &current_file_channel_header->y_axis_units, sizeof(current_file_channel_header->y_axis_units));
	memcpy(xUnits, &current_file_channel_header->x_axis_units, sizeof(current_file_channel_header->x_axis_units));
	memcpy(dataType, &current_file_channel_header->data_type, sizeof(current_file_channel_header->data_type));
	memcpy(dataKind, &current_file_channel_header->data_kind, sizeof(current_file_channel_header->data_kind));
	memcpy(spacing, &current_file_channel_header->space_between_elements_bytes, sizeof(current_file_channel_header->space_between_elements_bytes));
	memcpy(other, &current_file_channel_header->next_channel, sizeof(current_file_channel_header->next_channel));
}

void GetDSChan(cfs_short handle, cfs_short channel, WORD dataSect, TpLong chOffset, TpLong points, TpFloat yScale, TpFloat yOffset, TpFloat xScale, TpFloat xOffset)
{
	CFSFile *current_cfs_file = cfs_files[handle];
	CFSDSChannelHeader *current_ds_channel_header = get_ds_channel_header(current_cfs_file, channel, dataSect);
	memcpy(chOffset, &current_ds_channel_header->first_byte_offset, sizeof(current_ds_channel_header->first_byte_offset));
	memcpy(points, &current_ds_channel_header->data_points_count, sizeof(current_ds_channel_header->data_points_count));
	memcpy(yScale, &current_ds_channel_header->y_scale, sizeof(current_ds_channel_header->y_scale));
	memcpy(yOffset, &current_ds_channel_header->y_offset, sizeof(current_ds_channel_header->y_offset));
	memcpy(xScale, &current_ds_channel_header->x_increment, sizeof(current_ds_channel_header->x_increment));
	memcpy(xOffset, &current_ds_channel_header->x_offset, sizeof(current_ds_channel_header->x_offset));
}

// For the moment, just transfers everything
WORD GetChanData(cfs_short handle, cfs_short channel, WORD dataSect, cfs_long pointOff, WORD points, TpVoid dataADS, cfs_long areaSize)
{
	CFSFile *current_cfs_file = cfs_files[handle];
	CFSChannelData *current_channel_data = get_channel_data(current_cfs_file, channel, dataSect);

	int points_to_transfer;
	if (points == 0)
	{
		points_to_transfer = current_channel_data->data_points_count;
	}
	else
	{
		points_to_transfer = points;
	}

	if (pointOff > 0)
	{
		return 0;
	}

	int point_size = get_variable_size(current_channel_data->data_type);

	cfs_long size_to_transfer = point_size * points_to_transfer;

	if (size_to_transfer > areaSize)
	{
		return 0;
	}

	memcpy(dataADS, current_channel_data->data, size_to_transfer);

	return points_to_transfer;
}
