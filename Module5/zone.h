#pragma once

void Memory_Init(void* buffer, int bufferSize);

void* Hunk_Alloc(int requestSize);
void* Hunk_AllocName(int requestSize, char* requestName);
void* Hunk_HighAlloc(int requestSize);
void* Hunk_HighAllocName(int requestSize, char* requestName);

void Hunk_FreeToLowMark(int mark);
void Hunk_FreeToHighMark(int mark);