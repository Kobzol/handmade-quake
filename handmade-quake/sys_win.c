#include "quakedef.h"
#include "common.h"
#include "q_stl.h"
#include "winquake.h"

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
//		TIMER CODE
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