#include "host.h"
#include "common.h"

double realtime = 0;
double oldrealtime = 0;
double host_frametime = 0;

void Host_Init(void)
{
	COM_FileInit();
	VID_Init();
}

qboolean Host_FilterTime(float time)
{
	realtime += time;

	if (realtime - oldrealtime < 1.0 / 72.0)
	{
		return false;
	}

	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;

	return true;
}

void Host_Frame(float time)
{
	if (!Host_FilterTime(time))
	{
		return;
	}

	Sys_SendKeyEvents();

	VID_Update();

	// update game
	// render scene
}

void Host_Shutdown(void)
{
	VID_Shutdown();
	COM_FileShutdown();
}