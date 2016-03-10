#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

typedef struct
{
	char magic[4];
	int directoryOffset;
	int directoryLength;
} dpackheader_t;

typedef struct
{
	char fileName[56];
	int filePosition;
	int fileLength;
} dpackfile_t;

typedef struct
{
	char fileName[64];
	int filePosition;
	int fileLength;
} packfile_t;

typedef struct
{
	char packName[128];
	int packHandle;
	int numberOfFiles;
	packfile_t* packFiles;
} pack_t;

#define MAX_FILES_IN_PACK (2048)

pack_t* COM_LoadPackFile(char* pakFile)
{
	int pakSize;
	int pakHandle = Sys_FileOpenRead(pakFile, &pakSize);

	if (pakHandle < 0) return NULL;

	dpackheader_t header;
	Sys_FileRead(pakHandle, &header, sizeof(header));

	char packBuffer[5];
	memcpy(packBuffer, header.magic, sizeof(header.magic));
	packBuffer[4] = '\0';

	if (strcmp(packBuffer, "PACK"))
	{
		return NULL;
	}

	int numPackFiles = header.directoryLength / sizeof(dpackfile_t);
	packfile_t* filesInMemory = malloc(sizeof(packfile_t) * numPackFiles);

	Sys_FileSeek(pakHandle, header.directoryOffset);

	dpackfile_t tempPakFile[MAX_FILES_IN_PACK];
	Sys_FileRead(pakHandle, tempPakFile, header.directoryLength);

	for (int i = 0; i < numPackFiles; i++)
	{
		filesInMemory[i].filePosition = tempPakFile[i].filePosition;
		filesInMemory[i].fileLength = tempPakFile[i].fileLength;
		strcpy_s(filesInMemory[i].fileName, 64, tempPakFile[i].fileName);
	}

	pack_t* pak = malloc(sizeof(pack_t));
	pak->numberOfFiles = numPackFiles;
	pak->packHandle = pakHandle;
	pak->packFiles = filesInMemory;
	strcpy_s(pak->packName, 128, pakFile);

	return pak;
}

int main(int argc, char** argv)
{
	pack_t* pak0 = COM_LoadPackFile("id1/pak0.pak");
	pack_t* pak1 = COM_LoadPackFile("id1/pak1.pak");

	return 0;
}