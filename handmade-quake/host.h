#pragma once

#include "quakedef.h"
#include "vid.h"

void Host_Init(void);
qboolean Host_FilterTime(float time);
void Host_Frame(float time);
void Host_Shutdown(void);