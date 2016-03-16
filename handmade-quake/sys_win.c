#include "quakedef.h"
#include "common.h"
#include "q_stl.h"
#include "winquake.h"

#include <stdio.h>

#include "host.h"

static qboolean IsRunning = true;				// controls whether the game loop is running

HINSTANCE GlobalInstance;

void Sys_Shutdown(void)
{
	IsRunning = false;

	PostQuitMessage(0);
}
void Sys_SendKeyEvents(void)
{
	MSG message;
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

/////////////////////////////////
//		TIMER
/////////////////////////////////

static double GTimePassed = 0.0;			// total passed time from the start of the game in seconds
static double SecondsPerTick = 0.0;			// seconds passed per tick
static __int64 LastTick = 0;				// time of the last tick

// initializes the game time
float Sys_InitFloatTime(void)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	SecondsPerTick = 1.0 / frequency.QuadPart;

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	LastTick = time.QuadPart;

	return 0.0f;
}

float Sys_FloatTime(void)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	__int64 interval = time.QuadPart - LastTick;
	LastTick = time.QuadPart;

	double delta = interval * SecondsPerTick;
	GTimePassed += delta;

	return (float)GTimePassed;
}

/////////////////////////////////
//		FILE
/////////////////////////////////

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
		return (int)fread_s(dest, count, 1, count, file);
	}

	return -1;
}
int Sys_FileWrite(int handle, void* source, int count)
{
	if (handle < 0 || handle >= MAX_HANDLES) return -1;

	FILE* file = FileHandles[handle];

	if (file)
	{
		return (int)fwrite(source, 1, count, file);
	}

	return -1;
}

int32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nShowCmd)
{
	GlobalInstance = hInstance;

	COM_ParseCmdLine(lpCmdLine);

	Host_Init();

	float oldTime = Sys_InitFloatTime();

	while (IsRunning)
	{
		float newTime = Sys_FloatTime();
		Host_Frame(newTime - oldTime);
		oldTime = newTime;
	}

	Host_Shutdown();

	return 0;
}