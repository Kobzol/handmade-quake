#include <windows.h>
#include <stdint.h>
#include <string.h>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int32_t int32;
typedef uint32_t uint32;

#define MAX_NUM_ARGVS 50

int32 argc = 1;
uint8* largv[MAX_NUM_ARGVS + 1];

int32 Q_strcmp(uint8* left, uint8* right)
{
	while (*left == *right)
	{
		if (!*left) return 0;
		left++;
		right++;
	}

	return (*left < *right) ? -1 : 1;
}
int32 Q_atoi(uint8* str)
{
	int32 sign = 1;
	int32 value = 0;
	uint8 character;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}

	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))	// hexadecimal
	{
		str += 2;	// skip 0x

		while (1)
		{
			character = *str;
			str++;

			if (character >= '0' && character <= '9')
			{
				value = value * 16 + character - '0';
			}
			else if (character >= 'a' && character <= 'f')
			{
				value = value * 16 + 10 + character - 'a';
			}
			else if (character >= 'A' && character <= 'F')
			{
				value = value * 16 + 10 + character - 'A';
			}
			else return value * sign;
		}
	}
	else	// decimal
	{
		while (1)
		{
			character = *str;

			if (character < '0' || character > '9')
			{
				return value * sign;
			}
			else
			{
				value *= 10;
				value += character - '0';
				str++;
			}
		}
	}

	return value * sign;
}
int32 COM_CheckParm(uint8* parm)
{
	for (int32 i = 1; i < argc; i++)
	{
		if (Q_strcmp(parm, largv[i]) == 0)
		{
			return i;
		}
	}

	return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32 nCmdShow)
{
	largv[argc] = "";

	while (*lpCmdLine && argc < MAX_NUM_ARGVS + 1)
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

	int32 test = COM_CheckParm("-setalpha");

	return 0;
}