#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <time.h>

unsigned int Running = 1;

unsigned int WindowWidth = 800;
unsigned int WindowHeight = 600;

unsigned int BufferWidth = 320;
unsigned int BufferHeight = 240;
typedef unsigned int buffer_t;
unsigned int BytesPerPixel = sizeof(buffer_t);

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

	buffer_t* mem = buffer + (BufferWidth * y + x) * BytesPerPixel;  // move to first pixel

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

void DrawPic8(buffer_t* buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char* image)
{
	buffer_t* mem = buffer + (BufferWidth * y + x) * BytesPerPixel;  // move to first pixel

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			*mem = *image++;
			mem++;
		}
		mem += BufferWidth - width;  // skip row and move to first column
	}
}
void DrawPic32(buffer_t* buffer, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char* image)
{
	buffer_t* mem = buffer + (BufferWidth * y + x) * BytesPerPixel;  // move to first pixel

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			buffer_t color = *(buffer_t*) &BitMapInfo.acolors[*image++];
			*mem = color;
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
	}

	RECT windowInnerSize = { 0 };
	windowInnerSize.right = WindowWidth;
	windowInnerSize.bottom = WindowHeight;

	AdjustWindowRectEx(&windowInnerSize, windowStyle, 0, windowStyleEx);

	// window creation
	HWND mainWindow = CreateWindowEx(
		windowStyleEx,
		"Module 3",
		"Lesson 3.4",
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

	FILE* palette = fopen("palette.lmp", "rb");
	unsigned char paletteBuffer[256 * 3];
	size_t retval = fread(paletteBuffer, sizeof(char), 256 * 3, palette);
	unsigned char* paletteByte = paletteBuffer;

	for (int i = 0; i < 256; i++)
	{
		BitMapInfo.acolors[i].rgbRed = *paletteByte++;
		BitMapInfo.acolors[i].rgbGreen = *paletteByte++;
		BitMapInfo.acolors[i].rgbBlue = *paletteByte++;
	}

	fclose(palette);

	FILE* disc = fopen("DISC.lmp", "rb");
	int dimensions[2];
	retval = fread(dimensions, sizeof(int), 2, disc);

	unsigned char* discData = (unsigned char*) malloc(dimensions[0] * dimensions[1]);
	retval = fread(discData, sizeof(char), dimensions[0] * dimensions[1], disc);

	fclose(disc);

	FILE* pause = fopen("pause.lmp", "rb");
	int pauseDimensions[2];
	retval = fread(pauseDimensions, sizeof(int), 2, pause);

	unsigned char* pauseData = (unsigned char*) malloc(pauseDimensions[0] * pauseDimensions[1]);
	retval = fread(pauseData, sizeof(char), pauseDimensions[0] * pauseDimensions[1], pause);

	fclose(pause);

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
			DrawPic32(BackBuffer, 10, 10, dimensions[0], dimensions[1], discData);
			DrawPic32(BackBuffer, 10, 20, pauseDimensions[0], pauseDimensions[1], pauseData);
		}
		else
		{
			DrawPic8(BackBuffer, 10, 10, dimensions[0], dimensions[1], discData);
			DrawPic8(BackBuffer, 10, 20, pauseDimensions[0], pauseDimensions[1], pauseData);
		}

		StretchDIBits(hdc, 0, 0, WindowWidth, WindowHeight, 0, 0, BufferWidth, BufferHeight, BackBuffer, (BITMAPINFO*) &BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	DeleteDC(hdc);
	free(BackBuffer);

	free(pauseData);
	free(discData);

	return EXIT_SUCCESS;
}