#include <stdint.h>

// Constants to select between file variables and data section variables
#define FILEVAR 0
#define DSVAR 1

// Constants defining flag bits
#define FLAG7 1
#define FLAG6 2
#define FLAG5 4
#define FLAG4 8
#define FLAG3 16
#define FLAG2 32
#define FLAG1 64
#define FLAG0 128
#define FLAG15 256
#define FLAG14 512
#define FLAG13 1024
#define FLAG12 2048
#define FLAG11 4096
#define FLAG10 8192
#define FLAG9 16384
#define FLAG8 32768
#define noFlags 0

#define INT1 0 // 1 byte int
#define WRD1 1 // 1 byte unsigned int
#define INT2 2 // 2 byte int
#define WRD2 3 // 2 byte int
#define INT4 4 // 4 byte int
#define RL4 5 // 4 byte float
#define RL8 6 // 8 byte double
#define LSTR 7 // variable length string

// Constants defining different channel storage methods/data kinds
#define EQUALSPACED 0
#define MATRIX 1
#define SUBSIDIARY 2

// Constants defining max lengths for various parts of the CFS file
#define DESCCHARS 20 // description max length: 20 characters
#define FNAMECHARS 12 // filename max length: 12 characters
#define COMMENTCHARS 72 // comment max length: 72 characters
#define UNITCHARS 8 // units max length: 8 characters

typedef uint16_t TSFlags;
typedef char TDataType;
typedef char TCFSKind;

typedef char TDesc[DESCCHARS + 2];
typedef char TFilename[FNAMECHARS + 2];
typedef char TComment[COMMENTCHARS + 2];
typedef char TUnits[UNITCHARS + 2];

typedef struct
{
	TDesc varDesc; // Variable Description
	TDataType vType; // INT1,WRD1 etc.
	char zerobyte; // for MS Pascal compatibility
	TUnits varUnits; // variable units
	short vSize; // no. of characters if LSTR
} TVarDesc;

// Some function expect a certain number of bytes.
// CFS was originally written for 32-bit systems,
// so we have to redefine some of the types.
typedef int16_t cfs_short;
typedef int32_t cfs_long;

typedef uint16_t WORD;
typedef char *TpStr;
typedef cfs_short *TpShort;
typedef float *TpFloat;
typedef cfs_long *TpLong;
typedef void *TpVoid;
typedef TSFlags *TpFlags;
typedef TDataType *TpDType;
typedef TCFSKind *TpDKind;
typedef WORD *TpUShort;

// TODO: Implement these functions
cfs_short CloseCFSFile(cfs_short handle);
cfs_short OpenCFSFile(TpStr fName, cfs_short enableWr, cfs_short memTable);
void GetGenInfo(cfs_short handle, TpStr time, TpStr date, TpStr comment);
void GetFileInfo(cfs_short handle, TpShort channels, TpShort fileVars, TpShort DSVars, TpUShort dataSects);
void GetVarDesc(cfs_short handle, cfs_short varNo, cfs_short varKind, TpShort varSize, TpDType varType, TpStr units, TpStr about);
void GetVarVal(cfs_short handle, cfs_short varNo, cfs_short vaKind, WORD dataSect, TpVoid varADS);
void GetFileChan(cfs_short handle, cfs_short channel, TpStr chanName, TpStr yUnits, TpStr xUnits, TpDType dataType, TpDKind dataKind, TpShort spacing, TpShort other);
void GetDSChan(cfs_short handle, cfs_short channel, WORD dataSect, TpLong chOffset, TpLong points, TpFloat yScale, TpFloat yOffset, TpFloat xScale, TpFloat xOffset);
WORD GetChanData(cfs_short handle, cfs_short channel, WORD dataSect, cfs_long pointOff, WORD points, TpVoid dataADS, cfs_long areaSize);
cfs_long CFSFileSize(cfs_short handle);
cfs_long GetDSSize(cfs_short handle, WORD dataSect);
cfs_short ReadData(cfs_short handle, WORD dataSect, cfs_long byteOff, WORD bytes, TpVoid dataADS);
WORD DSFlagValue(int nflag);
void DSFlags(cfs_short handle, WORD dataSect, short setIt, TpFlags pflagSet);
cfs_short FileError(TpShort handleNo, TpShort procNo, TpShort errNo);
