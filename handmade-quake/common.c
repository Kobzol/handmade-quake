#include "common.h"

int32 COM_argc = 1;
char* COM_largv[COM_MAX_NUM_ARGVS + 1];

int32 COM_CheckParm(char* parm)
{
	for (int32 i = 1; i < COM_argc; i++)
	{
		if (Q_strcmp(parm, COM_largv[i]) == 0)
		{
			return i;
		}
	}

	return 0;
}
void COM_ParseCmdLine(char* lpCmdLine)
{
	COM_largv[COM_argc] = "";

	while (*lpCmdLine && COM_argc < COM_MAX_NUM_ARGVS + 1)
	{
		while (*lpCmdLine && ((*lpCmdLine < 33) || (*lpCmdLine > 126)))	// skip text characters
		{
			lpCmdLine++;
		}

		if (*lpCmdLine)
		{
			COM_largv[COM_argc++] = lpCmdLine;	// save argument location

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
}