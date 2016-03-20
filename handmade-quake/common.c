#include "common.h"
#include "sys.h"

#include <stdarg.h>

char* va(char* format, ...)
{
	va_list va;
	static char buffer[1024];

	va_start(va, format);
	vsprintf_s(buffer, sizeof(buffer), format, va);
	va_end(va);

	return buffer;
}

/////////////////////////////////
//		CMD LINE PARSING
/////////////////////////////////

int32 COM_argc = 1;
char* COM_largv[COM_MAX_NUM_ARGVS + 1];

int32 COM_CheckParm(char* parm)
{
	for (int32 i = 1; i < COM_argc; i++)
	{
		if (Q_strcmp(parm, COM_largv[i]) == 0)
		{
			return i;
		}
	}

	return 0;
}
void COM_ParseCmdLine(char* lpCmdLine)
{
	COM_largv[COM_argc] = "";

	while (*lpCmdLine && COM_argc < COM_MAX_NUM_ARGVS + 1)
	{
		while (*lpCmdLine && ((*lpCmdLine < 33) || (*lpCmdLine > 126)))	// skip text characters
		{
			lpCmdLine++;
		}

		if (*lpCmdLine)
		{
			COM_largv[COM_argc++] = lpCmdLine;	// save argument location

			while (*lpCmdLine && (*lpCmdLine > 32) && (*lpCmdLine < 127))	// skip white-space characters
			{
				lpCmdLine++;
			}

			if (*lpCmdLine)
			{
				*lpCmdLine = '\0';
				lpCmdLine++;
			}
		}
	}
}

/////////////////////////////////
//		FILE STRUCTURES
/////////////////////////////////

#pragma pack(push, 1)
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
#pragma pack(pop)

typedef struct searchpath_s
{
	pack_t* pack;
	struct searchpath_s* next;
} searchpath_t;

#define MAX_FILES_IN_PACK (2048)
static searchpath_t* COM_SearchPaths = NULL;

static pack_t* COM_LoadPackFile(char* pakFile)
{
	int pakSize;
	int pakHandle = Sys_FileOpenRead(pakFile, &pakSize);

	if (pakHandle < 0) return NULL;

	dpackheader_t header;
	Sys_FileRead(pakHandle, &header, sizeof(header));

	char packBuffer[5];
	memcpy(packBuffer, header.magic, sizeof(header.magic));
	packBuffer[4] = '\0';

	if (Q_strcmp(packBuffer, "PACK"))
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
		Q_strncpy(filesInMemory[i].fileName, tempPakFile[i].fileName, 64);
	}

	pack_t* pak = malloc(sizeof(pack_t));
	pak->numberOfFiles = numPackFiles;
	pak->packHandle = pakHandle;
	pak->packFiles = filesInMemory;
	Q_strncpy(pak->packName, pakFile, 128);

	return pak;
}

static void COM_AddGameDirectory(const char* directory)
{
	char buffer[128];

	for (int i = 0;; i++)
	{
		sprintf_s(buffer, 128, "%s/pak%d.pak", directory, i);
		pack_t* pack = COM_LoadPackFile(buffer);

		if (!pack)
		{
			break;
		}

		searchpath_t* newPath = (searchpath_t*)malloc(sizeof(searchpath_t));
		newPath->pack = pack;
		newPath->next = COM_SearchPaths;
		COM_SearchPaths = newPath;
	}
}
uint8* COM_FindFile(const char* fileName, uint32* length)
{
	if (!fileName) return NULL;

	searchpath_t* jogger = COM_SearchPaths;

	while (jogger != NULL)
	{
		pack_t* pack = jogger->pack;
		for (int i = 0; i < pack->numberOfFiles; i++)
		{
			packfile_t* packFile = pack->packFiles + i;
			if (!Q_strcmp(packFile->fileName, fileName))
			{
				Sys_FileSeek(pack->packHandle, packFile->filePosition);
				uint8* fileData = (uint8*) malloc(packFile->fileLength);
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

void COM_FileInit(void)
{
	COM_AddGameDirectory("id1");
}
void COM_FileShutdown(void)
{
	while (COM_SearchPaths)
	{
		searchpath_t* next = COM_SearchPaths->next;

		Sys_FileClose(COM_SearchPaths->pack->packHandle);
		free(COM_SearchPaths->pack->packFiles);
		free(COM_SearchPaths->pack);
		free(COM_SearchPaths);

		COM_SearchPaths = next;
	}
}
