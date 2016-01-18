#include "q_stl.h"

int32 Q_atoi(const char* str)
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
int32 Q_strcmp(const char* left, const char* right)
{
	while (*left == *right)
	{
		if (!*left) return 0;
		left++;
		right++;
	}

	return (*left < *right) ? -1 : 1;
}
void Q_strcpy(char* dest, const char* src)
{
	while (*src)
	{
		*dest++ = *src++;
	}

	*dest = '\0';
}
uint32 Q_strlen(const char* str)
{
	uint32 count = 0;

	while (*str++)
	{
		count++;
	}

	return count;
}
void Q_strncpy(char* dest, const char* src, uint32 count)
{
	if (count < 1) return;

	while (*src && count > 0)
	{
		*dest++ = *src++;
		count--;
	}

	while (count > 0)
	{
		*dest++ = '\0';
		count--;
	}
}