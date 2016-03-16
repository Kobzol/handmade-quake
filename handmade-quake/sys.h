#pragma once

void Sys_Shutdown(void);
void Sys_SendKeyEvents(void);

int Sys_FileOpenRead(char* path, int *size);
int Sys_FileOpenWrite(char* path);
void Sys_FileClose(int handle);
void Sys_FileSeek(int handle, int position);
int Sys_FileRead(int handle, void* dest, int count);
int Sys_FileWrite(int handle, void* source, int count);