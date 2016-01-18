#pragma once

#include "quakedef.h"

int32 Q_atoi(const char* str);
void Q_strcpy(char* dest, const char* src);
int32 Q_strcmp(const char* left, const char* right);
uint32 Q_strlen(const char* str);
void Q_strncpy(char* dest, const char* src, uint32 count);