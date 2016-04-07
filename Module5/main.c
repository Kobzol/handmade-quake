#include <stdlib.h>

#include "zone.h"

#define ALLOC_SIZE (16 * 1024 * 1024)

int main(int argc, char** argv)
{
	void* memory = malloc(ALLOC_SIZE);

	Memory_Init(memory, ALLOC_SIZE);

	Hunk_AllocName(1, "test");

	free(memory);

	return 0;
}