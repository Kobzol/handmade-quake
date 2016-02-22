#include "vid.h"

static HWND MainWindow;

static uint32 WindowHeight = 600;
static uint32 WindowWidth = 800;
static uint32 BufferWidth = 320;
static uint32 BufferHeight = 240;

void* BackBuffer = NULL;

BITMAPINFO BitMapInfo = { 0 };

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
			VID_SetMode(640, 480);
		}
		else if (wParam == 'S')
		{
			VID_SetMode(800, 600);
		}
		else if (wParam == 'D')
		{
			VID_SetMode(1024, 768);
		}
	}
		break;
	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

void VID_SetMode(uint32 width, uint32 height)
{
	RECT rectangle;
	GetWindowRect(MainWindow, &rectangle);
	MoveWindow(MainWindow, rectangle.left, rectangle.top, width, height, TRUE);

	WindowWidth = width;
	WindowHeight = height;
}

void VID_Init(void)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = GlobalInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "Module 2";

	if (!RegisterClassEx(&wc))
	{
		exit(EXIT_FAILURE);
	}

	DWORD windowStyleEx = 0;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	/*BOOL fullscreen;

	if (fullscreen)
	{
	DEVMODE screenSettings = { 0 };
	screenSettings.dmSize = sizeof(screenSettings);
	screenSettings.dmPelsWidth = WindowWidth;
	screenSettings.dmPelsHeight = WindowHeight;
	screenSettings.dmBitsPerPel = 32;
	screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
	{
	windowStyleEx = WS_EX_APPWINDOW;
	windowStyle = WS_POPUP;
	}
	else
	{
	Fullscreen = FALSE;
	}
	}*/

	RECT windowInnerSize = { 0 };
	windowInnerSize.right = WindowWidth;
	windowInnerSize.bottom = WindowHeight;

	AdjustWindowRectEx(&windowInnerSize, windowStyle, 0, windowStyleEx);

	uint32 width = windowInnerSize.right - windowInnerSize.left;
	uint32 height = windowInnerSize.bottom - windowInnerSize.top;

	MainWindow = CreateWindowEx(windowStyleEx, "Module 2", "Lesson 2.6", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, GlobalInstance, NULL);

	HDC deviceContext = GetDC(MainWindow);
	PatBlt(deviceContext, 0, 0, width, height, BLACKNESS);
	ReleaseDC(MainWindow, deviceContext);

	// bitmap info
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = -(int)BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 8;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight);
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