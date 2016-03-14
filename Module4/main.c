#include <stddef.h>
#include <stdio.h>
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

typedef struct searchpath_s
{
	pack_t* pack;
	struct searchpath_s* next;
} searchpath_t;

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

static searchpath_t* COM_SearchPaths = NULL;

void COM_AddGameDirectory(const char* directory)
{
	char buffer[128];

	for (int i = 0; ; i++)
	{
		sprintf_s(buffer, 128, "%s/pak%d.pak", directory, i);
		pack_t* pack = COM_LoadPackFile(buffer);
		
		if (!pack)
		{
			break;
		}

		searchpath_t* newPath = (searchpath_t*) malloc(sizeof(searchpath_t));
		newPath->pack = pack;
		newPath->next = COM_SearchPaths;
		COM_SearchPaths = newPath;
	}
}
char* COM_FindFile(const char* fileName, int* length)
{
	if (!fileName) return NULL;

	searchpath_t* jogger = COM_SearchPaths;

	while (jogger != NULL)
	{
		pack_t* pack = jogger->pack;
		for (int i = 0; i < pack->numberOfFiles; i++)
		{
			packfile_t* packFile = pack->packFiles + i;
			if (!strcmp(packFile->fileName, fileName))
			{
				Sys_FileSeek(pack->packHandle, packFile->filePosition);
				char* fileData = (char*) malloc(packFile->fileLength);
				Sys_FileRead(pack->packHandle, fileData, packFile->fileLength);

				if (length)
				{
					*length = packFile->fileLength;
				}

				return fileData;
			}
		}

		jogger = jogger->next;
	}

	return NULL;
}

int main(int argc, char** argv)
{
	COM_AddGameDirectory("id1");

	int length;
	char* paletteBits = COM_FindFile("gfx/palette.lmp", &length);

	return 0;
}