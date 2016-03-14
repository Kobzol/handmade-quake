#include "file.h"

#include <stdio.h>

#define MAX_HANDLES 10
static FILE* FileHandles[MAX_HANDLES] = { 0 };

static int FindHandle(void)
{
	for (int i = 0; i < MAX_HANDLES; i++)
	{
		if (FileHandles[i] == NULL)
		{
			return i;
		}
	}

	return -1;
}
static int FileLength(FILE* file)
{
	int pos = ftell(file);
	fseek(file, 0, SEEK_END);

	int end = ftell(file);
	fseek(file, pos, SEEK_SET);

	return end;
}

int Sys_FileOpenRead(char* path, int *size)
{
	int handle = FindHandle();
	if (handle < 0) return -1;

	errno_t result = fopen_s(&FileHandles[handle], path, "rb");

	if (!result)
	{
		if (size)
		{
			*size = FileLength(FileHandles[handle]);
		}

		return handle;
	}
	else return -1;
}
int Sys_FileOpenWrite(char* path)
{
	int handle = FindHandle();
	if (handle < 0) return -1;

	errno_t result = fopen_s(&FileHandles[handle], path, "wb");

	if (result) return -1;
	else return handle;
}
void Sys_FileClose(int handle)
{
	if (handle < 0 || handle >= MAX_HANDLES) return;

	FILE* file = FileHandles[handle];
	if (file)
	{
		int result = fclose(file);
		FileHandles[handle] = NULL;
	}
}
void Sys_FileSeek(int handle, int position)
{
	if (handle < 0 || handle >= MAX_HANDLES) return;

	FILE* file = FileHandles[handle];
	if (file)
	{
		fseek(file, position, SEEK_SET);
	}
}
int Sys_FileRead(int handle, void* dest, int count)
{
	if (handle < 0 || handle >= MAX_HANDLES) return -1;

	FILE* file = FileHandles[handle];

	if (file)
	{
		return (int) fread_s(dest, count, 1, count, file);
	}
	
	return -1;
}
int Sys_FileWrite(int handle, void* source, int count)
{
	if (handle < 0 || handle >= MAX_HANDLES) return -1;

	FILE* file = FileHandles[handle];

	if (file)
	{
		return (int) fwrite(source, 1, count, file);
	}

	return -1;
}