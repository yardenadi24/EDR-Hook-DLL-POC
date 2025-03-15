#include "pch.h"
#include "hook.h"
#include "memory.h"
#include <stdio.h>

// Helper function to check if the code contains padding bytes (0x00, 0x90, 0xCC)
static BOOL IsPaddingSequence(const LPBYTE pCode, SIZE_T length)
{
    // Empty sequence can't be padding
    if (length == 0)
        return FALSE;

    // Check if the first byte is one of the known padding bytes
    if (pCode[0] != 0x00 && pCode[0] != 0x90 && pCode[0] != 0xCC)
        return FALSE;

    // Padding should consist of repeated bytes of the same value
    for (SIZE_T i = 1; i < length; ++i) {
        if (pCode[i] != pCode[0])
            return FALSE;
    }

    return TRUE;
}

BOOL
HookCreateTrampoline(PTRAMPOLINE pTrampInfo)
{
    // Structure for absolute JMP instruction (used for relay function)
    JMP_ABS absJumpTemplate = {
        0xFF, 0x25, 0x00000000,   // JMP [RIP+6] (FF 25 00000000)
        0x0000000000000000ULL     // Target address placeholder
    };

    // Structure for CALL instruction to handle RIP-relative calls
    CALL_ABS absCallTemplate = {
        0xFF, 0x15, 0x00000002,   // CALL [RIP+8] (FF 15 00000002)
        0xEB, 0x08,               // JMP +10 (EB 08) - Skip over the address data
        0x0000000000000000ULL     // Target address placeholder
    };

    // Structure for conditional jumps (converted to absolute form)
    JCC_ABS condJumpTemplate = {
        0x70, 0x0E,               // Jcc +14 (7x 0E)
        0xFF, 0x25, 0x00000000,   // JMP [RIP+6] (FF 25 00000000)
        0x0000000000000000ULL     // Target address placeholder
    };

    // === STATE TRACKING VARIABLES ===
    UINT8 srcOffset = 0;          // Current offset in source function
    UINT8 destOffset = 0;         // Current offset in destination trampoline
    ULONG_PTR targetJumpAddr = 0; // Address of furthest internal jump target
    BOOL allInstructionsCopied = FALSE; // Flag indicating completion
    UINT8 instructionBuffer[16];  // Buffer for instruction modification

    // Initialize the trampoline state
    pTrampInfo->HotPatchable = FALSE;
    pTrampInfo->InstructionCount = 0;

    do {
        // Calculate the current instruction addresses
        ULONG_PTR currentSrcAddr = (ULONG_PTR)pTrampInfo->TargetFunction + srcOffset;
        ULONG_PTR currentDestAddr = (ULONG_PTR)pTrampInfo->TrampolineBuffer + destOffset;

        // Variables for the current instruction
        x86_instruction_t instruction;
        SIZE_T bytesToCopy;
        LPVOID copySource;


        // Disassemble the current instruction
        bytesToCopy = x86_disassembler((BYTE*)currentSrcAddr, &instruction);
        if (instruction.flags & F_ERROR) {
            // Failed to decode instruction - cannot reliably hook
            return FALSE;
        }

        // Default behavior is to copy the original instruction
        copySource = (LPVOID)currentSrcAddr;

        // Check if we've reached the minimum bytes needed for the jump
        if (srcOffset >= sizeof(JMP_REL)) {
            // We've gathered enough instructions for the hook
            // Create a jump back to the original function (after the hook)
            absJumpTemplate.address = currentSrcAddr;
            copySource = &absJumpTemplate;
            bytesToCopy = sizeof(absJumpTemplate);
            allInstructionsCopied = TRUE;
        }
        // Handle special instruction types that need adjustments
        else if (MODRM_IS_RELATIVE(instruction.modrm))
        {

            // RIP-relative addressing needs adjustment for the new location
            // (examples include instructions like: MOV RAX, [RIP+offset], LEA RCX, [RIP+offset])
            PUINT32 relativeAddr;

            // Copy the instruction to modify it
            CopyMemory(instructionBuffer, (LPBYTE)currentSrcAddr, bytesToCopy);
            copySource = instructionBuffer;

            // Calculate the position of the displacement value
            relativeAddr = (PUINT32)(instructionBuffer + instruction.length - instruction.imm_size - 4);

            // Adjust the offset to reflect the new trampoline location
            // The formula calculates the absolute target address and then the relative offset from new location
            *relativeAddr = (UINT32)(((currentSrcAddr + instruction.length) +
                (INT32)instruction.disp.disp32) - (currentDestAddr + instruction.length));

            // If this is an indirect jump (FF /4), we can consider the function complete
            if (instruction.opcode == 0xFF && instruction.modrm_reg == 4)
                allInstructionsCopied = TRUE;


        }
        else if (DIRECT_RELATIVE_CALL_OPCODE(instruction.opcode))
        {
            // Direct CALL instruction (E8) needs conversion to absolute form
            ULONG_PTR callTarget = currentSrcAddr + instruction.length + (INT32)instruction.imm.imm32;
            absCallTemplate.address = callTarget;
            copySource = &absCallTemplate;
            bytesToCopy = sizeof(absCallTemplate);
        }
        else if (DIRECT_RELATIVE_JMP_OPCODE(instruction.opcode))
        {
            // Handle direct JMP instructions (E9 for near, EB for short)
            ULONG_PTR jumpTarget = currentSrcAddr + instruction.length;

            if (instruction.opcode == 0xEB) // short jump
                jumpTarget += (INT8)instruction.imm.imm8;
            else // near jump
                jumpTarget += (INT32)instruction.imm.imm32;

            // Internal jump within the function we're hooking
            if ((ULONG_PTR)pTrampInfo->TargetFunction <= jumpTarget &&
                jumpTarget < ((ULONG_PTR)pTrampInfo->TargetFunction + sizeof(JMP_REL))) {
                // Update the furthest target if needed
                if (targetJumpAddr < jumpTarget)
                    targetJumpAddr = jumpTarget;
            }
            else {
                // External jump - convert to absolute form
                absJumpTemplate.address = jumpTarget;
                copySource = &absJumpTemplate;
                bytesToCopy = sizeof(absJumpTemplate);

                // Finish unless we still need to process instructions for internal jumps
                allInstructionsCopied = (currentSrcAddr >= targetJumpAddr);
            }

        }
        else if (DIRECT_RELATIVE_LOOP_COND_OPCODE(instruction.opcode) ||
                DIRECT_RELATIVE_LOOP_COND_OPCODE2(instruction.opcode2))
        {
                
            // Handle conditional jumps and loop instructions
            ULONG_PTR jumpTarget = currentSrcAddr + instruction.length;

            if ((instruction.opcode & 0xF0) == 0x70 || (instruction.opcode & 0xFC) == 0xE0) {
                // 8-bit offset (70-7F: Jcc, E0-E3: LOOP/LOOPE/LOOPNE/JCXZ)
                jumpTarget += (INT8)instruction.imm.imm8;
            }
            else {
                // 32-bit offset (0F 80-8F: Jcc)
                jumpTarget += (INT32)instruction.imm.imm32;
            }

            // Check if it's an internal jump
            if ((ULONG_PTR)pTrampInfo->TargetFunction <= jumpTarget &&
                jumpTarget < ((ULONG_PTR)pTrampInfo->TargetFunction + sizeof(JMP_REL))) {
                // Update furthest target if needed
                if (targetJumpAddr < jumpTarget)
                    targetJumpAddr = jumpTarget;
            }
            else if ((instruction.opcode & 0xFC) == 0xE0) {
                // Cannot handle LOOP/LOOPZ/LOOPNZ/JCXZ/JECXZ to external targets
                return FALSE;
            }
            else {
                // Convert conditional jump to absolute form
                UINT8 condition = ((instruction.opcode != 0x0F ? instruction.opcode : instruction.opcode2) & 0x0F);
                // Invert condition code (e.g., JE becomes JNE) for our two-part jump
                condJumpTemplate.opcode = 0x71 ^ condition;
                condJumpTemplate.address = jumpTarget;
                copySource = &condJumpTemplate;
                bytesToCopy = sizeof(condJumpTemplate);
            }

        }
        else if (RET_OPCODE(instruction.opcode))
        {
            // RET instruction - can finish trampoline if not inside a branch
            allInstructionsCopied = (currentSrcAddr >= targetJumpAddr);
        }

        // Verify that we can process this instruction inside a branch
        if (currentSrcAddr < targetJumpAddr && bytesToCopy != instruction.length)
            return FALSE;

        // Ensure the trampoline won't exceed the maximum size
        if ((destOffset + bytesToCopy) > TRAMPOLINE_MAX_SIZE)
            return FALSE;

        // Ensure we don't exceed instruction mapping capacity
        if (pTrampInfo->InstructionCount >= ARRAYSIZE(pTrampInfo->OriginalOffsets))
            return FALSE;

        // Record instruction mapping for debugging and detour management
        pTrampInfo->OriginalOffsets[pTrampInfo->InstructionCount] = srcOffset;
        pTrampInfo->TrampolineOffsets[pTrampInfo->InstructionCount] = destOffset;
        pTrampInfo->InstructionCount++;

        CopyMemory((LPBYTE)pTrampInfo->TrampolineBuffer + destOffset, copySource, bytesToCopy);

        // Advance the offsets
        destOffset += bytesToCopy;
        srcOffset += instruction.length;

    } while (!allInstructionsCopied);

 
  
    // === HOT PATCH ANALYSIS ===
    // Determine if we need to use hot patching technique
    BOOL standardJumpImpossible = (srcOffset < sizeof(JMP_REL) &&
        !IsPaddingSequence((LPBYTE)pTrampInfo->TargetFunction + srcOffset, sizeof(JMP_REL) - srcOffset));

    if (standardJumpImpossible)
    {
        // Try to use short jump if standard won't fit
        BOOL shortJumpImpossible = (srcOffset < sizeof(JMP_REL_SHORT) &&
            !IsPaddingSequence((LPBYTE)pTrampInfo->TargetFunction + srcOffset, sizeof(JMP_REL_SHORT) - srcOffset));

        if (shortJumpImpossible)
            return FALSE; // Can't hook this function - not enough space

        // Check if we can use hot patching (patch code before function)
        LPBYTE preTargetMem = (LPBYTE)pTrampInfo->TargetFunction - sizeof(JMP_REL);

        // Memory before function must be executable and consist of code padding
        if (!IsExecutableAddress(preTargetMem) ||
            !IsPaddingSequence(preTargetMem, sizeof(JMP_REL)))
            return FALSE;

        // We can use hot patching
        pTrampInfo->HotPatchable = TRUE;
    }

    // === RELAY FUNCTION CREATION ===
    // Create the relay function that jumps to the detour
    absJumpTemplate.address = (ULONG_PTR)pTrampInfo->DetourFunction;
    pTrampInfo->RelayFunction = (LPBYTE)pTrampInfo->TrampolineBuffer + destOffset;
    CopyMemory(pTrampInfo->RelayFunction, &absJumpTemplate, sizeof(absJumpTemplate));

    return TRUE;
}


BOOL WINAPI HookCreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal)
{
    // Allocate a memory block close to the target function
    LPVOID pBufferMem = GetMemoryBlock(pTarget);
    if (pBufferMem == NULL)
        return FALSE;


    // Initialize the trampoline structure
    TRAMPOLINE trampolineInfo;
    trampolineInfo.TargetFunction = pTarget;
    trampolineInfo.DetourFunction = pDetour;
    trampolineInfo.TrampolineBuffer = pBufferMem;

    // Create the trampoline function
    if (!HookCreateTrampoline(&trampolineInfo))
        return FALSE;

    // Return the trampoline address as the original function pointer
    *ppOriginal = trampolineInfo.TrampolineBuffer;

    // === APPLY THE HOOK ===
    // Determine patch location and size based on hot patching status
    SIZE_T patchSize = sizeof(JMP_REL);
    LPBYTE pPatchTarget = (LPBYTE)pTarget;

    // Adjust patch location for hot patching
    if (trampolineInfo.HotPatchable) {
        pPatchTarget -= sizeof(JMP_REL);
        patchSize += sizeof(JMP_REL_SHORT);
    }

    // TODO:: Backup for uninstall

    // Change memory protection to allow writing to the code
    DWORD oldProtect;
    if (!VirtualProtect(pPatchTarget, patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
        return FALSE;

    // Write the appropriate hook instructions
    if (trampolineInfo.HotPatchable) {
        // Hot patching approach:
        // 1. Write short jump at function start to jump backward to long jump
        PJMP_REL_SHORT pShortJmp = (PJMP_REL_SHORT)pTarget;
        pShortJmp->Opcode = 0xEB;  // JMP rel8
        pShortJmp->Operand = (UINT8)(0 - (sizeof(JMP_REL_SHORT) + sizeof(JMP_REL)));

        // 2. Write long jump before the function to jump to the relay
        PJMP_REL pJmp = (PJMP_REL)pPatchTarget;
        pJmp->Opcode = 0xE9;  // JMP rel32

        // Calculate relative offset for jump
        ULONG_PTR targetAddr = (ULONG_PTR)trampolineInfo.RelayFunction;
        ULONG_PTR sourceAddr = (ULONG_PTR)pPatchTarget + sizeof(JMP_REL);
        INT32 offset = (INT32)(targetAddr - sourceAddr);
        pJmp->Operand = offset;

        // Debug output
#ifdef _DEBUG
        char dbgMsg[100];
        sprintf_s(dbgMsg, "Hot patch jump from 0x%p to 0x%p, offset: 0x%08X",
            (void*)pPatchTarget, (void*)trampolineInfo.RelayFunction, offset);
        OutputDebugStringA(dbgMsg);
#endif
    }
    else {
        // Standard approach - just place a single jump
        PJMP_REL pJmp = (PJMP_REL)pPatchTarget;
        pJmp->Opcode = 0xE9;  // JMP rel32

        // Calculate relative offset for jump
        ULONG_PTR targetAddr = (ULONG_PTR)trampolineInfo.RelayFunction;
        ULONG_PTR sourceAddr = (ULONG_PTR)pPatchTarget + sizeof(JMP_REL);
        INT32 offset = (INT32)(targetAddr - sourceAddr);
        pJmp->Operand = offset;

        // Debug output
#ifdef _DEBUG
        char dbgMsg[100];
        sprintf_s(dbgMsg, "Standard jump from 0x%p to 0x%p, offset: 0x%08X",
            (void*)pPatchTarget, (void*)trampolineInfo.RelayFunction, offset);
        OutputDebugStringA(dbgMsg);
#endif
    }

    // Restore memory protection
    VirtualProtect(pPatchTarget, patchSize, oldProtect, &oldProtect);

    return TRUE;
}