#include <stdint.h>
#include <stdio.h>

typedef int16_t cfs_short;
typedef int32_t cfs_long;
typedef uint8_t BYTE;
typedef uint16_t cfs_word;

typedef enum
{
	INT1 = 0,
	WRD1 = 1,
	INT2 = 2,
	WRD2 = 3,
	INT4 = 4,
	RL4 = 5,
	RL8 = 6,
	LSTR = 7,
} CFSDataType;

typedef enum
{
	EQUALSPACED = 0,
	MATRIX = 1,
	SUBSIDIARY = 2,
} CFSDataKind;

typedef struct
{
	char file_id[8];
	char file_name[14];
	cfs_long file_size;
	char file_creation_time[8];
	char file_creation_date[8];
	cfs_short channel_count;
	cfs_short file_variable_count;
	cfs_short data_section_variable_count;
	cfs_short file_header_size_bytes;
	cfs_short data_section_header_size_bytes;
	cfs_long final_data_section_header_offset;
	cfs_word data_section_count;	
	cfs_word disk_block_size_rounding;
	char file_comment[74];
	cfs_long pointer_table_offset;
	BYTE reserved_space[40];	
} CFSHeader;

typedef struct
{
	char channel_name[22];
	char y_axis_units[10];
	char x_axis_units[10];
	BYTE data_type;
	BYTE data_kind;
	cfs_short space_between_elements_bytes;
	cfs_short next_channel;
} ChannelHeader;

void print_cfs_header(CFSHeader *cfs_header);
void print_channel_header(ChannelHeader *channel_header);
void read_cfs_header(FILE *file, CFSHeader *cfs_header);
void read_channel_header(FILE *file, ChannelHeader *channel_header);

