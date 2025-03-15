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
 * JMP_REL_SHORT - Short Relative Jump (2 bytes)
 * Used for small jumps within ±127 bytes
 */
typedef struct JMP_REL_SHORT
{
    UINT8 Opcode;   // EB: Short jump opcode
    UINT8 Operand;  // 8-bit signed displacement
} JMP_REL_SHORT, * PJMP_REL_SHORT;

/**
 * JMP_REL - Standard Relative Jump (5 bytes)
 * Used for jumps within ±2GB
 */
typedef struct JMP_REL
{
    UINT8  Opcode;  // E9: Near relative jump opcode
    UINT32 Operand; // 32-bit signed displacement
} JMP_REL, * PJMP_REL;

/**
 * CALL_REL - Relative Call (5 bytes)
 * Similar to JMP_REL but pushes return address
 */
typedef struct CALL_REL
{
    UINT8  Opcode;  // E8: Near relative call opcode
    UINT32 Operand; // 32-bit signed displacement
} CALL_REL, * PCALL_REL;

/**
 * JMP_ABS - Absolute Indirect Jump (14 bytes)
 * Used for 64-bit addressing without limitations
 */
typedef struct JMP_ABS
{
    UINT8  opcode0; // FF: Indirect jump opcode
    UINT8  opcode1; // 25: ModR/M for RIP-relative indirect
    UINT32 dummy;   // 00000000: Zero displacement
    UINT64 address; // 64-bit absolute target address
} JMP_ABS, * PJMP_ABS;

/**
 * CALL_ABS - Absolute Indirect Call (16 bytes)
 * Complex instruction for calls with 64-bit addressing
 */
typedef struct CALL_ABS
{
    UINT8  opcode0; // FF: Indirect call opcode
    UINT8  opcode1; // 15: ModR/M for RIP-relative indirect
    UINT32 dummy0;  // 00000002: Points to address field
    UINT8  dummy1;  // EB: Short jump to skip address data
    UINT8  dummy2;  // 08: Jump 8 bytes forward
    UINT64 address; // 64-bit absolute target address
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

/**
 * JCC_ABS - Absolute Conditional Jump (16 bytes)
 * Used to convert relative conditional jumps to absolute form
 */
typedef struct JCC_ABS
{
    UINT8  opcode;  // 7x: Conditional jump opcode
    UINT8  dummy0;  // 0E: Jump offset past absolute jump
    UINT8  dummy1;  // FF: Indirect jump opcode
    UINT8  dummy2;  // 25: ModR/M for RIP-relative indirect
    UINT32 dummy3;  // 00000000: Zero displacement
    UINT64 address; // 64-bit absolute target address
} JCC_ABS, * PJCC_ABS;
// ------------------ Instructions structures for hooking ------------------------ //

#pragma pack(pop)

// ------------------ Trampoline structures for hooking ------------------------ //
/**
 * TRAMPOLINE - Structure for hook management
 * Contains all information needed for hooking a function
 */
typedef struct _TRAMPOLINE
{
    LPVOID TargetFunction;   // Original function to be hooked
    LPVOID DetourFunction;   // Hook implementation function
    LPVOID TrampolineBuffer; // Buffer containing trampoline code
    LPVOID RelayFunction;    // Entry point to the detour
    BOOL   HotPatchable;     // Whether to use hot patching technique
    UINT   InstructionCount; // Number of copied instructions
    UINT8  OriginalOffsets[8];    // Source offsets of copied instructions
    UINT8  TrampolineOffsets[8];  // Destination offsets in trampoline
} TRAMPOLINE, * PTRAMPOLINE;
// ------------------ Trampoline structures for hooking ------------------------ //


// ------------------ Hook control structures for hooking ------------------------ //

// TODO: Use Hook structure to control installed hooks
//       And then be able to uninstall them.
typedef struct _HOOK
{
    PVOID TargetFunction;    // Original function
    PVOID DetourFunction;    // Hook implementation
    PTRAMPOLINE Trampoline;  // Trampoline data
    BYTE OriginalBytes[32];  // Backup of overwritten bytes
    DWORD StolenBytesLen;    // Number of bytes overwritten
    BOOL Installed;          // Whether hook is active
} HOOK, * PHOOK;
// ------------------ Hook control structures for hooking ------------------------ //


/**
 * CreateFunctionTrampoline - Creates a trampoline for the target function
 *
 * Analyzes the target function instructions and creates a trampoline that:
 * 1. Executes the instructions that will be overwritten by the hook
 * 2. Jumps back to the original function after the hook point
 * 3. Contains a relay function to redirect execution to the detour
 *
 * @param pTrampInfo Trampoline information structure
 * @return TRUE if trampoline created successfully, FALSE otherwise
 */

PVOID
HookCreateTrampoline(
    PVOID pOriginalFunction,
    PVOID pDetourFunction,
    BYTE* StolenBytes,
    SIZE_T CountBytes);


/**
 * HookCreateHook - Main function to install a function hook
 *
 * @param pTarget Function to be hooked
 * @param pDetour Function that will be called instead
 * @param ppOriginal Receives pointer to trampoline for calling original function
 * @return TRUE if hook installed successfully, FALSE otherwise
 */

BOOL
WINAPI
HookCreateHook(
    LPVOID pTarget,
    LPVOID pDetour,
    LPVOID* ppOriginal);



/*
    Hook Process Overview:

    1. Allocate memory close to target function
    2. Analyze and copy enough instructions to create a trampoline
    3. Place a jump at the original function start to redirect to hook
    4. Return a pointer to the trampoline for calling the original functionality

    Memory Layout:
    +-----------------+     +-------------------+     +----------------+
    | Target Function |     | Trampoline Buffer |     | Detour Function|
    | (Original Code) |     |                   |     | (Your Hook)    |
    | +-------------+ |     | +---------------+ |     |                |
    | | JMP to relay| |     | | Copied instr. | |     |                |
    | +-------------+ | --> | +---------------+ |     |                |
    | | Rest of     | |     | | JMP to orig.  | | <---|                |
    | | function    | | <-- | +---------------+ |     |                |
    | +-------------+ |     | | Relay:        | |     |                |
    |                 |     | | JMP to detour | | --> |                |
    +-----------------+     +-------------------+     +----------------+
*/