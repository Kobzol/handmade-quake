#pragma once

#include "quakedef.h"
#include "q_stl.h"

#define COM_MAX_NUM_ARGVS 50

int32 COM_CheckParm(char* parm);
void COM_ParseCmdLine(char* lpCmdLine);

void COM_FileInit(void);
uint8* COM_FindFile(const char* fileName, uint32* length);
void COM_FileShutdown(void);