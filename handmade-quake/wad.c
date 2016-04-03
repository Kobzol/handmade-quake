#include "wad.h"
#include "common.h"

typedef struct
{
	int8 magic[4];
	int32 numberOfLumps;
	int32 directoryOffset;
} wadinfo_t;

typedef struct
{
	int32 filePosition;
	int32 diskSize;
	int32 size;
	int8 fileType;
	int8 compressionType;
	int16 padding;
	int8 name[16];	// zero-padded
} lumpinfo_t;

static int8* WadFile;
static int32 LumpCount;
static int32 FileOffset;
static lumpinfo_t* Lumps;

static W_CleanupName(int8* in, int8* out)
{
	int i = 0;
	for (; i < 16; i++)
	{
		int8 ch = in[i];
		
		if (!ch) break;

		if (ch >= 'A' && ch <= 'Z')
		{
			ch += ('a' - 'A');
		}

		out[i] = ch;
	}

	for (; i < 16; i++)
	{
		out[i] = '\0';
	}
}
static lumpinfo_t* W_GetLumpInfo(int8* name)
{
	int8 cleanName[16];
	W_CleanupName(name, cleanName);

	lumpinfo_t* lumpWalker = Lumps;
	for (int i = 0; i < LumpCount; i++)
	{
		if (!Q_strcmp(lumpWalker->name, cleanName))
		{
			return lumpWalker;
		}

		lumpWalker++;
	}

	assert(false);
	return NULL;
}

void W_LoadWadFile(const int8* filename)
{
	WadFile = COM_FindFile(filename, NULL);
	assert(WadFile);

	wadinfo_t* header = (wadinfo_t*) WadFile;

	char packBuffer[5];
	memcpy(packBuffer, header->magic, sizeof(header->magic));
	packBuffer[4] = '\0';

	assert(!Q_strcmp(packBuffer, "WAD2"));

	LumpCount = header->numberOfLumps;
	FileOffset = header->directoryOffset;

	Lumps = (lumpinfo_t*) (WadFile + FileOffset);
	lumpinfo_t* lumpWalker = Lumps;

	for (int i = 0; i < LumpCount; i++)
	{
		W_CleanupName(lumpWalker->name, lumpWalker->name);
		lumpWalker++;
	}
}
void* W_GetLumpName(int8* name)
{
	lumpinfo_t* lump = W_GetLumpInfo(name);

	if (lump)
	{
		return WadFile + lump->filePosition;
	}
	else return NULL;
}
