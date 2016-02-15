#include <Windows.h>
#include <time.h>

unsigned int Running = 1;

unsigned int BufferWidth = 640;
unsigned int BufferHeight = 480;
unsigned int BytesPerPixel = 1;
typedef unsigned char buffer_t;

buffer_t* BackBuffer = NULL;
BOOL Fullscreen = FALSE;

typedef struct
{
	BITMAPINFOHEADER	bmiHeader;
	RGBQUAD				acolors[256];
} dibinfo_t;

dibinfo_t BitMapInfo = { 0 };

void DrawRect(buffer_t* buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height,
	unsigned char red, unsigned char green, unsigned char blue)
{
	if (x >= BufferWidth) return;
	if (y >= BufferHeight) return;

	if (x + width >= BufferWidth)
	{
		width = BufferWidth - x - 1;
	}
	if (y + height >= BufferHeight)
	{
		height = BufferHeight - y - 1;
	}

	unsigned int color = (red << 16) | (green << 8) | blue;
	buffer_t* mem = buffer;

	mem += (BufferWidth * y + x) * BytesPerPixel;  // move to first pixel

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			*mem = color;
			mem++;
		}
		mem += BufferWidth - width;  // skip row and move to first column
	}
}
void DrawRect8(buffer_t* buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height,
	buffer_t colorIndex)
{
	if (x >= BufferWidth) return;
	if (y >= BufferHeight) return;

	if (x + width >= BufferWidth)
	{
		width = BufferWidth - x - 1;
	}
	if (y + height >= BufferHeight)
	{
		height = BufferHeight - y - 1;
	}

	buffer_t* mem = buffer;

	mem += (BufferWidth * y + x) * BytesPerPixel;  // move to first pixel

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			*mem = colorIndex;
			mem++;
		}
		mem += BufferWidth - width;  // skip row and move to first column
	}
}

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
	srand((unsigned int) time(NULL));

	// window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "Module 3";

	RegisterClassExA(&wc);

	DWORD windowStyleEx = 0;
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;

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
	BitMapInfo.bmiHeader.biHeight = - (int) BufferHeight;
	BitMapInfo.bmiHeader.biPlanes = 1;
	BitMapInfo.bmiHeader.biBitCount = 8 * BytesPerPixel;
	BitMapInfo.bmiHeader.biCompression = BI_RGB;

	BackBuffer = malloc(BufferWidth * BufferHeight * BytesPerPixel);

	if (BytesPerPixel == 1)
	{
		BitMapInfo.acolors[0].rgbRed = 0;
		BitMapInfo.acolors[0].rgbGreen = 0;
		BitMapInfo.acolors[0].rgbBlue = 0;

		for (int i = 1; i < 256; i++)
		{
			BitMapInfo.acolors[i].rgbRed = rand() % 256;
			BitMapInfo.acolors[i].rgbGreen = rand() % 256;
			BitMapInfo.acolors[i].rgbBlue = rand() % 256;
		}
	}

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

		buffer_t* bufferMemory = BackBuffer;
		for (unsigned int y = 0; y < BufferHeight; y++)
		{
			for (unsigned int x = 0; x < BufferWidth; x++)
			{
				*bufferMemory++ = rand() % 256;
			}
		}

		if (BytesPerPixel == 4)
		{
			DrawRect(BackBuffer, 10, 10, 300, 200, 255, 0, 0);
		}
		else DrawRect8(BackBuffer, 10, 10, 300, 150, 1);

		StretchDIBits(hdc, 0, 0, BufferWidth, BufferHeight, 0, 0, BufferWidth, BufferHeight, BackBuffer, (BITMAPINFO*) &BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	DeleteDC(hdc);
	free(BackBuffer);

	return EXIT_SUCCESS;
}