#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int32_t bool;

#undef true
#undef false

typedef enum { false, true } qboolean;