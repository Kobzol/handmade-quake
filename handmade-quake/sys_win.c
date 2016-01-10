#include <windows.h>
#include <string.h>

#define MAX_NUM_ARGVS 50

int argc = 1;
char* largv[MAX_NUM_ARGVS];


int COM_CheckParm(char* parm)
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(parm, largv[i]) == 0)
		{
			return i;
		}
	}

	return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	largv[argc] = "";

	while (*lpCmdLine && argc < MAX_NUM_ARGVS)
	{
		while (*lpCmdLine && ((*lpCmdLine < 33) || (*lpCmdLine > 126)))	// skip text characters
		{
			lpCmdLine++;
		}

		if (*lpCmdLine)
		{
			largv[argc++] = lpCmdLine;	// save argument location

			while (*lpCmdLine && (*lpCmdLine > 32) && (*lpCmdLine < 127))	// skip white-space characters
			{
				lpCmdLine++;
			}

			if (*lpCmdLine)
			{
				*lpCmdLine = '\0';
				lpCmdLine++;
			}
		}
	}

	if (argc < MAX_NUM_ARGVS)
	{
		largv[argc] = "";
	}

	int test = COM_CheckParm("-setalpha");

	return 0;
}