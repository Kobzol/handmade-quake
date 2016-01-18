#include "common.h"
#include "quakedef.h"
#include "q_stl.h"
#include "winquake.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
	COM_ParseCmdLine(lpCmdLine);

	int32 test = COM_CheckParm("-setalpha");

	return 0;
}