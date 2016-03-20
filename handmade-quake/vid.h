#pragma once

#include "quakedef.h"
#include "winquake.h"
#include "sys.h"

typedef enum
{
	MS_WINDOWED,
	MS_FULLSCREEN
} modestate_t;

typedef struct
{
	modestate_t type;
	uint32 width;
	uint32 height;
	uint32 frequency;
} vmode_t;

typedef uint32 backbuffer_t;

typedef struct
{
	backbuffer_t* BackBuffer;
	backbuffer_t* ColorData;
	uint32 BufferWidth;
	uint32 BufferHeight;
} viddef_t;

extern viddef_t Vid;

void VID_Init(void);
void VID_InitWindowedMode(void);
void VID_InitFullscreenMode(void);
void VID_SetMode(uint32 mode_index);
void VID_Update(void);
void VID_Shutdown(void);