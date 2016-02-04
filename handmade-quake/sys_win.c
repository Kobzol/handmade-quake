#include "quakedef.h"
#include "common.h"
#include "q_stl.h"
#include "winquake.h"

#include "host.h"

static qboolean IsRunning = true;				// controls whether the game loop is running

void Sys_Shutdown(void)
{
	IsRunning = false;

	PostQuitMessage(0);
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

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
	case WM_ACTIVATE:
		break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		Sys_Shutdown();
		break;
	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nShowCmd)
{
	COM_ParseCmdLine(lpCmdLine);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 2";

	if (!RegisterClass(&wc))
	{
		return EXIT_FAILURE;
	}

	HWND mainWindow;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	int32 width = 800;
	int32 height = 600;

	RECT rectangle = { 0 };
	rectangle.bottom = height;
	rectangle.right = width;

	AdjustWindowRect(&rectangle, windowStyle, 0);

	width = rectangle.right - rectangle.left;
	height = rectangle.bottom - rectangle.top;

	mainWindow = CreateWindowEx(0, "Module 2", "Lesson 2.6", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	HDC deviceContext = GetDC(mainWindow);
	PatBlt(deviceContext, 0, 0, width, height, BLACKNESS);
	ReleaseDC(mainWindow, deviceContext);

	Host_Init();

	float oldTime = Sys_InitFloatTime();

	MSG message;

	while (IsRunning)
	{
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		float newTime = Sys_FloatTime();
		Host_Frame(newTime - oldTime);
		oldTime = newTime;
	}

	Host_Shutdown();

	return 0;
}