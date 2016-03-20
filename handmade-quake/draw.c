#include "draw.h"

void DrawPic32(uint32 x, uint32 y, uint32 width, uint32 height, uint8* image)
{
	backbuffer_t* mem = Vid.BackBuffer + (Vid.BufferWidth * y + x);  // move to first pixel

	for (uint32 i = 0; i < height; i++)
	{
		for (uint32 j = 0; j < width; j++)
		{
			if (*image != 0xFF)
			{
				backbuffer_t color = Vid.ColorData[*image];
				*mem = color;
			}

			image++;
			mem++;
		}
		mem += Vid.BufferWidth - width;  // skip row and move to first column
	}
}