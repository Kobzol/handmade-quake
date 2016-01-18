#pragma once

#include "quakedef.h"
#include "q_stl.h"

#define COM_MAX_NUM_ARGVS 50

extern int32 COM_argc;
extern char* COM_largv[COM_MAX_NUM_ARGVS + 1];

int32 COM_CheckParm(char* parm);
void COM_ParseCmdLine(char* lpCmdLine);