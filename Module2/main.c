#include <windows.h>

BOOL IsRunning = TRUE;



LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
		case WM_KEYUP:
			IsRunning = FALSE;
			break;
		case WM_DESTROY:
			IsRunning = FALSE;
			break;
		default:
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
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

	int width = 800;
	int height = 600;

	RECT rectangle = { 0 };
	rectangle.bottom = height;
	rectangle.right = width;

	AdjustWindowRect(&rectangle, windowStyle, 0);

	width = rectangle.right - rectangle.left;
	height = rectangle.bottom - rectangle.top;

	mainWindow = CreateWindowEx(0, "Module 2", "Lesson 2.1", windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	HDC deviceContext = GetDC(mainWindow);
	PatBlt(deviceContext, 0, 0, width, height, BLACKNESS);
	ReleaseDC(mainWindow, deviceContext);

	MSG message;

	while (IsRunning)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return 0;
}