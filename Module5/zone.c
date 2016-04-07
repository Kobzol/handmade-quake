#include "zone.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define HUNK_SENTINEL (0x1DF001ED)

typedef struct
{
	int memorySentinel;
	int memorySize;
	char name[8];
} hunk_t;

// hunk
static char* HunkBase;
static int HunkSize;
static int HunkLowOffset;
static int HunkHighOffset;

// zone
#define ZONE_SIZE (0xC000)

void Memory_Init(void* buffer, int bufferSize)
{
	assert(buffer);

	HunkBase = buffer;
	HunkSize = bufferSize;
	HunkLowOffset = 0;
	HunkHighOffset = 0;

	// allocate the zone
	//void* zoneSpace = Hunk_AllocName(ZONE_SIZE, "zone");
}

void* Hunk_AllocName(int requestSize)
{
	return Hunk_AllocName(requestSize, "unknown");
}
void* Hunk_AllocName(int requestSize, char* requestName)
{
	int totalSize = sizeof(hunk_t) + ((requestSize + 15) & ~0xF);	// requestSize = (requestSize + ((16 - (requestSize % 16) % 16))

	//assert(HunkLowOffset + totalSize < HunkHighOffset);

	hunk_t* Header = (hunk_t*) (HunkBase + HunkLowOffset);
	HunkLowOffset += totalSize;

	memset(Header, 0, totalSize);
	Header->memorySize = totalSize;
	Header->memorySentinel = HUNK_SENTINEL;
	
	if (requestName)
	{
		strcpy_s(Header->name, sizeof(Header->name), requestName);
	}

	return Header + 1;	// skip the header
}