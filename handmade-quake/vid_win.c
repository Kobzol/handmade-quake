#include "vid.h"

static HWND MainWindow;

static uint32 WindowHeight = 600;
static uint32 WindowWidth = 800;
static uint32 BufferWidth = 320;
static uint32 BufferHeight = 240;

static void* BackBuffer = NULL;

static BITMAPINFO BitMapInfo = { 0 };
static vmode_t Modelist[50];
static uint32 ModeCount = 0;
static uint32 ModeActive = 0;
static uint32_t FirstFullscreenMode = 0;

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
	case WM_KEYDOWN:
	{
		if (wParam == 'A')
		{
			VID_SetMode(0);
		}
		else if (wParam == 'S')
		{
			VID_SetMode(FirstFullscreenMode);
		}
		else if (wParam == 'D')
		{
			VID_SetMode(2);
		}
	}
		break;
	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

void VID_SetMode(uint32_t mode_index)
{
	uint32 width = Modelist[mode_index].width;
	uint32 height = Modelist[mode_index].height;

	vmode_t mode = Modelist[mode_index];

	if (mode.type == MS_FULLSCREEN)
	{
		DEVMODE screenSettings = { 0 };
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = width;
		screenSettings.dmPelsHeight = height;
		screenSettings.dmBitsPerPel = 32;
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
	}
	else
	{
		if (Modelist[ModeActive].type == MS_FULLSCREEN)
		{
			ChangeDisplaySettings(NULL, 0);
		}

		RECT rectangle;
		GetWindowRect(MainWindow, &rectangle);
		MoveWindow(MainWindow, rectangle.left, rectangle.top, width, height, TRUE);
	}

	WindowWidth = width;
	WindowHeight = height;

	ModeActive = mode_index;
}

void VID_Init(void)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = GlobalInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 3";

	if (!RegisterClassEx(&wc))
	{
		exit(EXIT_FAILURE);
	}

	DWORD windowStyleEx = 0;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	RECT windowInnerSize = { 0 };
	windowInnerSize.right = WindowWidth;
	windowInnerSize.bottom = WindowHeight;

	AdjustWindowRectEx(&windowInnerSize, windowStyle, 0, windowStyleEx);

	uint32 width = windowInnerSize.right - windowInnerSize.left;
	uint32 height = windowInnerSize.bottom - windowInnerSize.top;

	MainWindow = CreateWindowEx(windowStyleEx, "Module 3", "Lesson 3.6", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, GlobalInstance, NULL);

	HDC deviceContext = GetDC(MainWindow);
	PatBlt(deviceContext, 0, 0, width, height, BLACKNESS);
	ReleaseDC(MainWindow, deviceContext);

	// bitmap info
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = - (int32) BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 8;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight);

	VID_InitWindowedMode();
	VID_InitFullscreenMode();

	VID_SetMode(0);
}
void VID_InitWindowedMode(void)
{
	Modelist[ModeCount].type = MS_WINDOWED;
	Modelist[ModeCount].width = 320;
	Modelist[ModeCount].height = 240;
	Modelist[ModeCount].frequency = 60;
	ModeCount++;

	Modelist[ModeCount].type = MS_WINDOWED;
	Modelist[ModeCount].width = 640;
	Modelist[ModeCount].height = 480;
	Modelist[ModeCount].frequency = 60;
	ModeCount++;

	Modelist[ModeCount].type = MS_WINDOWED;
	Modelist[ModeCount].width = 800;
	Modelist[ModeCount].height = 600;
	Modelist[ModeCount].frequency = 60;
	ModeCount++;

	Modelist[ModeCount].type = MS_WINDOWED;
	Modelist[ModeCount].width = 1024;
	Modelist[ModeCount].height = 768;
	Modelist[ModeCount].frequency = 60;
	ModeCount++;
}

void VID_InitFullscreenMode(void)
{
	FirstFullscreenMode = ModeCount;

	DEVMODE devmode;
	uint32 modeNum = 0;
	BOOL success;
	uint32 last_width = 0, last_height = 0;

	do
	{
		success = EnumDisplaySettings(NULL, modeNum++, &devmode);

		if (!success)
		{
			break;
		}

		if (devmode.dmDisplayFrequency >= 59)
		{
			if (devmode.dmPelsWidth != last_width || devmode.dmPelsHeight != last_height)
			{
				Modelist[ModeCount].type = MS_FULLSCREEN;
				Modelist[ModeCount].width = devmode.dmPelsWidth;
				Modelist[ModeCount].height = devmode.dmPelsHeight;
				Modelist[ModeCount].frequency = devmode.dmDisplayFrequency;
				ModeCount++;

				last_width = devmode.dmPelsWidth;
				last_height = devmode.dmPelsHeight;
			}
			else if (devmode.dmDisplayFrequency > Modelist[ModeCount - 1].frequency)
			{
				Modelist[ModeCount - 1].frequency = devmode.dmDisplayFrequency;
			}
		}

	} while (success);
}

void VID_Update(void)
{
	HDC hdc = GetDC(MainWindow);
	StretchDIBits(hdc, 0, 0, WindowWidth, WindowHeight, 0, 0, BufferWidth, BufferHeight, BackBuffer, &BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(MainWindow, hdc);
}

void VID_Shutdown(void)
{
	free(BackBuffer);
	BackBuffer = NULL;
}