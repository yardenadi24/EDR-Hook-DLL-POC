#pragma once
#include "Windows.h"

#define MEMORY_SLOT_SIZE 64

LPVOID GetMemoryBlock(LPVOID pOrigin);
BOOL   IsExecutableAddress(LPVOID pAddress);