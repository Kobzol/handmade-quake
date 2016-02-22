#pragma once

#include "quakedef.h"
#include "winquake.h"
#include "sys.h"

void VID_Init(void);
void VID_SetMode(uint32 width, uint32 height);
void VID_Update(void);
void VID_Shutdown(void);