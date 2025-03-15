#pragma once
#include "Windows.h"
#include "x86_instruction.h"

// Size of the absolute jump instruction
#define ABS_JMP_SIZE 14
#define ABS_JMP_OPCODE 0xFF // JMP r/m64
#define ABS_JMP_MODRM  0X25 // ModR/M byte: 00 100 101 (Mod=0, Reg=4 for JMP, R/M=5 for RIP-relative)
#define MEM_PROTECT_SIZE 32

#define MODRM_IS_RELATIVE(b) ((b & 0xC7) == 0x05)
#define DIRECT_RELATIVE_CALL_OPCODE(b) (b == 0xE8)
#define DIRECT_RELATIVE_JMP_OPCODE(b) (b == 0xEB || b == 0xE9)
#define DIRECT_RELATIVE_LOOP_COND_OPCODE(b) ( (b & 0xF0) == 0x70 || (b & 0xFC) == 0xE0 )
#define DIRECT_RELATIVE_LOOP_COND_OPCODE2(b)  ((b & 0xF0) == 0x80)
#define RET_OPCODE(b) ((b & 0xFE) == 0xC2)

#define MEMORY_SLOT_SIZE 64
#define TRAMPOLINE_MAX_SIZE (MEMORY_SLOT_SIZE - sizeof(JMP_ABS))


// ------------------ Instructions structures for hooking ------------------------ //
#pragma pack(push, 1)
/**
 * JMP_REL_SHORT - 8-bit Short Relative Jump (2 bytes total)
 * Used for jumping to nearby locations (±127 bytes)
 */
typedef struct JMP_REL_SHORT
{
    UINT8 Opcode;   // EB: Short jump opcode
    UINT8 Operand;  // 8-bit signed displacement relative to the next instruction
    // Range: -128 to +127 bytes from the end of this instruction
} JMP_REL_SHORT, * PJMP_REL_SHORT;

/**
 * JMP_REL - 32-bit Relative Jump (5 bytes total)
 * Standard jump used for most hook installations
 */
typedef struct JMP_REL
{
    UINT8  Opcode;  // E9: Near relative jump opcode
    UINT32 Operand; // 32-bit signed displacement relative to the next instruction
    // Allows jumping ±2GB from the end of this instruction
    // Formula: target_address - (current_address + 5)
} JMP_REL, * PJMP_REL;

/**
 * CALL_REL - 32-bit Relative Call (5 bytes total)
 * Similar to JMP_REL but pushes return address to stack
 */
typedef struct CALL_REL
{
    UINT8  Opcode;  // E8: Near relative call opcode
    UINT32 Operand; // 32-bit signed displacement relative to the next instruction
    // Used when you need to return to the calling location
    // Formula: target_address - (current_address + 5)
} CALL_REL, * PCALL_REL;

/**
 * JMP_ABS - 64-bit Indirect Absolute Jump (14 bytes total)
 * Used when target is beyond ±2GB range in 64-bit mode
 */
typedef struct JMP_ABS
{
    UINT8  opcode0; // FF: Group 5 opcode (includes indirect jump variants)
    UINT8  opcode1; // 25: ModR/M byte specifying "JMP [RIP+disp32]" (absolute indirect)
    UINT32 dummy;   // 00000000: 32-bit zero displacement (points to the address field below)
    UINT64 address; // Absolute 64-bit destination address to jump to
    // CPU will read this address and jump to it (memory indirect)
} JMP_ABS, * PJMP_ABS;

/**
 * CALL_ABS - 64-bit Indirect Absolute Call (16 bytes total)
 * Complex structure used for absolute calls in 64-bit mode
 */
typedef struct CALL_ABS
{
    UINT8  opcode0; // FF: Group 5 opcode (includes indirect call variants)
    UINT8  opcode1; // 15: ModR/M byte specifying "CALL [RIP+disp32]" (absolute indirect)
    UINT32 dummy0;  // 00000002: 32-bit displacement pointing to address field
    UINT8  dummy1;  // EB: Short jump opcode (to skip over the address)
    UINT8  dummy2;  // 08: Jump forward 8 bytes (skips over the address field)
    // This ensures execution doesn't fall into the address data
    UINT64 address; // Absolute 64-bit destination address for the call
    // When called, first instruction reads this address and calls it
} CALL_ABS, * PCALL_ABS;

/**
 * JCC_REL - 32-bit Conditional Jump (6 bytes total)
 * Used for conditional jumps in hook trampolines
 */
typedef struct JCC_REL
{
    UINT8  opcode0; // 0F: Two-byte opcode prefix
    UINT8  opcode1; // 8*: Second opcode byte determining condition (80-8F)
    // Examples: 0F 84 = JE, 0F 85 = JNE, 0F 8C = JL, etc.
    UINT32 operand; // 32-bit signed displacement relative to the next instruction
    // Used to implement conditional logic in trampolines
    // Formula: target_address - (current_address + 6)
} JCC_REL, * PJCC_REL;

// 64bit indirect absolute conditional jumps that x64 lacks.
typedef struct _JCC_ABS
{
    UINT8  opcode;      // 7* 0E:         J** +16
    UINT8  dummy0;
    UINT8  dummy1;      // FF25 00000000: JMP [+6]
    UINT8  dummy2;
    UINT32 dummy3;
    UINT64 address;     // Absolute destination address
} JCC_ABS, *PJCC_ABS;
// ------------------ Instructions structures for hooking ------------------------ //

#pragma pack(pop)

// ------------------ Trampoline structures for hooking ------------------------ //
typedef struct _TRAMPOLINE
{
	LPVOID pTarget; // Address of the target function
	LPVOID pDetour; // Address of the detour function
	LPVOID pTrampoline; // Buffer address for the trampoline and relay function
	LPVOID pRelay; // Address for the relay function
	BOOL HotPathArea;       // [Out] Should  use the hot patch area
    UINT   nIP;             // [Out] Number of the instruction boundaries.
	UINT8  oldIPs[8];       // [Out] Instruction boundaries of the target function.
	UINT8  newIPs[8];       // [Out] Instruction boundaries of the trampoline function.
} TRAMPOLINE, *PTRAMPOLINE;
// ------------------ Trampoline structures for hooking ------------------------ //


// ------------------ Hook control structures for hooking ------------------------ //
typedef struct _HOOK
{
	PVOID TargetFunction;
	PVOID DetourFunction;
	PTRAMPOLINE Trampoline;
	BYTE OriginalBytes[32];
	DWORD StolenBytesLen;
	BOOL Installed;
} HOOK, *PHOOK;
// ------------------ Hook control structures for hooking ------------------------ //

PVOID
HookCreateTrampoline(
    PVOID pOriginalFunction,
    PVOID pDetourFunction,
    BYTE* StolenBytes,
    SIZE_T CountBytes);

BOOL
WINAPI
HookCreateHook(
    LPVOID pTarget,
    LPVOID pDetour,
    LPVOID* ppOriginal);



/*
    Memory Layout :
        +-----------------+     +-------------------+     +---------------- +
        | Target Function |     | Trampoline Buffer  |    | Detour Function |
        | (Original Code) |    | +-------------- +  |    | (Your Hook)     |
        | +------------ + |    | | Copied       |   |    |                 |
        | | JMP rel32   | -- + --->| | Instructions |    |    |             |
        | +------------ + |    | +-------------- +  |    |                 |
        | | Rest of     |  |    | | JMP to rest  |  |    |                |
        | | Function    | < -+---- + -| of original |  |    |            |
        | +------------ + |    | +-------------- +  |    |                 |
        |                 |    | | Relay:       |   |    |                 |
        |                 |    | | JMP absolute |-- + --->|              |
        +---------------- - +| +-------------- + |      +---------------- +
            +------------------ - +
*/