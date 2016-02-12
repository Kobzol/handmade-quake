#include <Windows.h>

int Running = 1;

int BufferWidth = 640;
int BufferHeight = 480;

void* BackBuffer = NULL;

BITMAPINFO BitMapInfo = { 0 };

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
	case WM_CLOSE:
		Running = 0;
		break;
	default:
		result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return result;
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "Module 3";

	RegisterClassExA(&wc);

	DWORD windowStyleEx = 0;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;

	BOOL Fullscreen = TRUE;

	if (Fullscreen)
	{
		DEVMODE screenSettings = { 0 };
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = BufferWidth;
		screenSettings.dmPelsHeight = BufferHeight;
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
	}

	RECT windowInnerSize = { 0 };
	windowInnerSize.right = BufferWidth;
	windowInnerSize.bottom = BufferHeight;

	AdjustWindowRectEx(&windowInnerSize, windowStyle, 0, windowStyleEx);

	// window creation
	HWND mainWindow = CreateWindowEx(
		windowStyleEx,
		"Module 3",
		"Lesson 3.2",
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowInnerSize.right - windowInnerSize.left,
		windowInnerSize.bottom - windowInnerSize.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (Fullscreen)
	{
		SetWindowLong(mainWindow, GWL_STYLE, 0);
	}

	ShowWindow(mainWindow, nShowCmd);

	// bitmap info
	BitMapInfo.bmiHeader.biSize = sizeof(BitMapInfo.bmiHeader);
	BitMapInfo.bmiHeader.biWidth = BufferWidth;
	BitMapInfo.bmiHeader.biHeight = BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 32;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight * 4);

	// message loop
	MSG message;
	HDC hdc = GetDC(mainWindow);	

	while (Running)
	{
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		unsigned int* bufferMemory = (unsigned int*)BackBuffer;
		for (int y = 0; y < BufferHeight; y++)
		{
			for (int x = 0; x < BufferWidth; x++)
			{
				unsigned int red = rand() % 256;
				unsigned int green = rand() % 256;
				unsigned int blue = rand() % 256;

				*bufferMemory++ = (red << 16) | (green << 8) | blue;
			}
		}

		StretchDIBits(hdc, 0, 0, BufferWidth, BufferHeight, 0, 0, BufferWidth, BufferHeight, BackBuffer, &BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	DeleteDC(hdc);
	free(BackBuffer);

	return EXIT_SUCCESS;
}