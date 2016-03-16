#pragma once

#include "quakedef.h"
#include "q_stl.h"

#define COM_MAX_NUM_ARGVS 50

int32 COM_CheckParm(char* parm);
void COM_ParseCmdLine(char* lpCmdLine);

void COM_FileInit(void);
char* COM_FindFile(const char* fileName, int* length);
void COM_FileShutdown(void);