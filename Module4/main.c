#include <stddef.h>

#include "file.h"

int main(int argc, char** argv)
{
	int size;
	int fileHandle = Sys_FileOpenRead("main.c", &size);

	if (fileHandle == -1)
	{

	}

	char buffer[1024];
	size_t read = Sys_FileRead(fileHandle, buffer, sizeof(buffer));

	int writeFile = Sys_FileOpenWrite("main.out");
	Sys_FileWrite(writeFile, buffer, read);

	Sys_FileClose(fileHandle);
	Sys_FileClose(writeFile);

	return 0;
}