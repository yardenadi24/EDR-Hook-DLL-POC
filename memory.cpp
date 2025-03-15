#include "memory.h"
#include "pch.h"


// Size of each memory block. (= page size of VirtualAlloc)
#define MEMORY_BLOCK_SIZE 0x1000

// Max range for seeking a memory block. (= 1024MB)
#define MAX_MEMORY_RANGE 0x40000000

// Memory protection flags to check the executable address.
#define PAGE_EXECUTE_FLAGS (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

BOOL IsExecutableAddress(LPVOID pAddress)
{
	MEMORY_BASIC_INFORMATION mi;
	VirtualQuery(pAddress, &mi, sizeof(mi));

	return (mi.State == MEM_COMMIT && (mi.Protect & PAGE_EXECUTE_FLAGS));
}

static LPVOID FindPrevFreeRegion(LPVOID pAddress, LPVOID pMinAddr, DWORD dwAllocationGranularity)
{
	ULONG_PTR tryAddr = (ULONG_PTR)pAddress;

	// Round down to the allocation granularity.
	tryAddr -= tryAddr % dwAllocationGranularity;

	// Start from the previous allocation granularity multiply.
	tryAddr -= dwAllocationGranularity;

	while (tryAddr >= (ULONG_PTR)pMinAddr)
	{
		MEMORY_BASIC_INFORMATION mbi;

		// If failed the call break.
		if (VirtualQuery((LPVOID)tryAddr, &mbi, sizeof(mbi)) == 0)
			break;

		// If the memory is we can take it.
		if (mbi.State == MEM_FREE)
			return (LPVOID)tryAddr;
		// Memory is not free, if the base address of the allocation
		// Is less then the allocation block size we cant skip it back, break.
		if ((ULONG_PTR)mbi.AllocationBase < dwAllocationGranularity)
			break;

		// Jump back Allocation block of size from the Base to reach to the Previuse base.
		tryAddr = (ULONG_PTR)mbi.AllocationBase - dwAllocationGranularity;
	}

	return NULL;
}

static LPVOID FindNextFreeRegion(LPVOID pAddress, LPVOID pMaxAddr, DWORD dwAllocationGranularity)
{
	ULONG_PTR tryAddr = (ULONG_PTR)pAddress;

	// Round down to the allocation granularity.
	tryAddr -= tryAddr % dwAllocationGranularity;

	// Start from the next allocation granularity multiply.
	tryAddr += dwAllocationGranularity;

	while (tryAddr <= (ULONG_PTR)pMaxAddr)
	{
		MEMORY_BASIC_INFORMATION mbi;
		// If failed the call break.
		if (VirtualQuery((LPVOID)tryAddr, &mbi, sizeof(mbi)) == 0)
			break;

		if (mbi.State == MEM_FREE)
			return (LPVOID)tryAddr;

		// Skip the corrent region which is not free
		tryAddr = (ULONG_PTR)mbi.BaseAddress + mbi.RegionSize;

		// Round up to the next allocation granularity.
		tryAddr += dwAllocationGranularity - 1;
		tryAddr -= tryAddr % dwAllocationGranularity;
	}

	return NULL;
}

LPVOID GetMemoryBlock(LPVOID pOrigin)
{
	ULONG_PTR MinAddr = MAXDWORD64;
	ULONG_PTR MaxAddr = MAXDWORD64 + 1;

	LPVOID pBlock = NULL;

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	MinAddr = (ULONG_PTR)si.lpMinimumApplicationAddress;
	MaxAddr = (ULONG_PTR)si.lpMaximumApplicationAddress;

	// pOrigin +- 512MB
	// Adjust boundries of search
	if ((ULONG_PTR)pOrigin > MAX_MEMORY_RANGE && MinAddr < (ULONG_PTR)pOrigin - MAX_MEMORY_RANGE)
		MinAddr = (ULONG_PTR)pOrigin - MAX_MEMORY_RANGE;

	if(MaxAddr > (ULONG_PTR)pOrigin + MAX_MEMORY_RANGE)
		MaxAddr = (ULONG_PTR)pOrigin + MAX_MEMORY_RANGE;

	// Make room for MEMORY_BLOCK_SIZE bytes.
	MaxAddr -= MEMORY_BLOCK_SIZE - 1;

	LPVOID pAlloc = pOrigin;
	while ((ULONG_PTR)pAlloc >= MinAddr)
	{
		pAlloc = FindPrevFreeRegion(pAlloc, (LPVOID)MinAddr, si.dwAllocationGranularity);
		if (pAlloc == NULL)
			break;

		pBlock = VirtualAlloc(
			pAlloc, MEMORY_BLOCK_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (pBlock != NULL)
			break;
	}

	// Alloc a new block below if not found.
	if (pBlock == NULL)
	{
		LPVOID pAlloc = pOrigin;
		while ((ULONG_PTR)pAlloc <= MaxAddr)
		{
			pAlloc = FindNextFreeRegion(pAlloc, (LPVOID)MaxAddr, si.dwAllocationGranularity);
			if (pAlloc == NULL)
				break;

			pBlock = VirtualAlloc(
				pAlloc, MEMORY_BLOCK_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (pBlock != NULL)
				break;
		}
	}

	return pBlock;
}

