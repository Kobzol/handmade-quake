#pragma once

void Sys_Shutdown(void);
void Sys_SendKeyEvents(void);

int32 Sys_FileOpenRead(uint8* path, uint32 *size);
int32 Sys_FileOpenWrite(uint8* path);
void Sys_FileClose(int32 handle);
void Sys_FileSeek(int32 handle, uint32 position);
int32 Sys_FileRead(int32 handle, void* dest, uint32 count);
int32 Sys_FileWrite(int32 handle, void* source, uint32 count);