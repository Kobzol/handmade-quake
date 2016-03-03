#include <stdio.h>
#include <stdarg.h>

char* va(char* format, ...)
{
	va_list va_lst;
	va_start(va_lst, format);

	static char buffer[1024];

	vsnprintf_s(buffer, sizeof(buffer), format, va_lst);

	va_end(va_lst);

	return buffer;
}

int main(int argc, char** argv)
{
	char* str = va("%f", 5.0f);
	return 0;
}