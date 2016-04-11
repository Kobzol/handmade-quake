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
static char* HunkEnd;
static int HunkSize;
static int HunkLowOffset;
static int HunkHighOffset;

// zone
#define ZONE_SIZE (0xC000)

void Memory_Init(void* buffer, int bufferSize)
{
	assert(buffer);

	HunkBase = buffer;
	HunkEnd = HunkBase + bufferSize;
	HunkSize = bufferSize;
	HunkLowOffset = 0;
	HunkHighOffset = 0;

	// allocate the zone
	//void* zoneSpace = Hunk_AllocName(ZONE_SIZE, "zone");
}

void Hunk_Check(void)
{
	for (char* header = HunkBase; header != HunkBase + HunkLowOffset;)
	{
		hunk_t* hunkHeader = (hunk_t*) header;
		assert(hunkHeader->memorySentinel == HUNK_SENTINEL);
		assert(hunkHeader->memorySize >= 16);
		assert(header + hunkHeader->memorySize <= HunkEnd);

		header += hunkHeader->memorySize;
	}
}

void* Hunk_Alloc(int requestSize)
{
	return Hunk_AllocName(requestSize, "unknown");
}
void* Hunk_AllocName(int requestSize, char* requestName)
{
	int totalSize = sizeof(hunk_t) + ((requestSize + 15) & ~0xF);	// requestSize = (requestSize + ((16 - (requestSize % 16) % 16))

	assert(HunkBase + HunkLowOffset + totalSize < HunkEnd - HunkHighOffset);

	hunk_t* Header = (hunk_t*) (HunkBase + HunkLowOffset);
	HunkLowOffset += totalSize;

	memset(Header, 0, totalSize);
	Header->memorySize = totalSize;
	Header->memorySentinel = HUNK_SENTINEL;
	
	if (requestName)
	{
		strncpy_s(Header->name, sizeof(Header->name), requestName, sizeof(Header->name) - 1);
	}

	return Header + 1;	// skip the header
}
void* Hunk_HighAlloc(int requestSize)
{
	return Hunk_HighAllocName(requestSize, "unknown");
}
void* Hunk_HighAllocName(int requestSize, char* requestName)
{
	int totalSize = sizeof(hunk_t) + ((requestSize + 15) & ~0xF);	// requestSize = (requestSize + ((16 - (requestSize % 16) % 16))

	assert(HunkBase + HunkLowOffset < HunkEnd - HunkHighOffset - totalSize);

	HunkHighOffset += totalSize;	// move to the beginning of the newly placed header
	hunk_t* Header = (hunk_t*)(HunkEnd - HunkHighOffset);

	memset(Header, 0, totalSize);
	Header->memorySize = totalSize;
	Header->memorySentinel = HUNK_SENTINEL;

	if (requestName)
	{
		strncpy_s(Header->name, sizeof(Header->name), requestName, sizeof(Header->name) - 1);
	}

	return Header + 1;	// skip the header
}

void Hunk_FreeToLowMark(int mark)
{
	memset(HunkBase + mark, 0, HunkLowOffset - mark);
	HunkLowOffset = mark;
}
void Hunk_FreeToHighMark(int mark)
{
	memset(HunkEnd - HunkHighOffset, 0, HunkHighOffset - mark);
	HunkHighOffset = mark;
}
