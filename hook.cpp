#include "pch.h"
#include "hook.h"
#include "memory.h"
#include <stdio.h>

static BOOL IsCodePadding(LPBYTE pInst, UINT size)
{
    UINT i;

    if (pInst[0] != 0x00 && pInst[0] != 0x90 && pInst[0] != 0xCC)
        return FALSE;

    for (i = 1; i < size; ++i)
    {
        if (pInst[i] != pInst[0])
            return FALSE;
    }
    return TRUE;
}

BOOL
HookCreateTrampoline(PTRAMPOLINE ct)
{
    // Init structs
    CALL_ABS Call = {
        0xFF, 0x15, 0x00000002, // FF15 00000002: CALL [RIP+8]
        0xEB, 0x08,             // EB 08:         JMP +10
        0x0000000000000000ULL   // Absolute destination address
    };
    JMP_ABS Jmp = {
        0xFF, 0x25, 0x00000000, // FF25 00000000: JMP [RIP+6]
        0x0000000000000000ULL   // Absolute destination address
    };
    JCC_ABS Jcc = {
        0x70, 0x0E,             // 7* 0E:         J** +16
        0xFF, 0x25, 0x00000000, // FF25 00000000: JMP [RIP+6]
        0x0000000000000000ULL   // Absolute destination address
    };  

    // Initialize
    UINT8 OldPos = 0;       // How much bytes we already went over
    UINT8 NewPos = 0;
    ULONG_PTR JmpDest = 0;  // Destination address of an internal jump.
    BOOL Finished = FALSE;  // Is the function completed?
    UINT8 InstBuff[16];

    ct->HotPathArea = FALSE;
    ct->nIP = 0;

    do {

        x86_instruction_t hs;
        UINT CopySize;
        LPVOID pCopySrc;
        ULONG_PTR pOldInst = (ULONG_PTR)ct->pTarget + OldPos;
        ULONG_PTR pNewInst = (ULONG_PTR)ct->pTrampoline + NewPos;

        CopySize = x86_disassembler((BYTE*)pOldInst, &hs);
        if (hs.flags & F_ERROR)
        {
            // Failed parsing -> failed to hook
            return FALSE;
        }

        pCopySrc = (LPVOID)pOldInst;
        if (OldPos >= sizeof(JMP_REL))
        {
            // We have enough space for relative jump
            // No need to parse more instructions
            // Set the absolute jump back to the target function
            // From the trampoline

            Jmp.address = pOldInst;
            pCopySrc = &Jmp;
            CopySize = sizeof(Jmp);
            Finished = TRUE;

        }
        else if (MODRM_IS_RELATIVE(hs.modrm))
        {
            /*
                Detects RIP-relative addressing in memory operands
                Checks for ModR/M pattern: mod=00 and r/m=101
                Examples: MOV RAX, [RIP+offset], LEA RCX, [RIP+offset]
            */

            // Modify RIP relative address
            PUINT32 pRelAddr;

            // Copy the instruction to modify it
            CopyMemory(InstBuff, (LPBYTE)pOldInst, CopySize);
            pCopySrc = InstBuff;

            // Go to the end of the instruction,
            // subtract the immediate bytes, then subtract the disposition
            // Now the pointer is in the beginning of the disposition
            pRelAddr = (PUINT32)((InstBuff + hs.length) - hs.imm_size - 4);
            
            // Adjust the offset to be the subtraction
            // of the absolute address from our trampoline instruction location
            *pRelAddr = (UINT32)(((pOldInst + hs.length) + (INT32)hs.disp.disp32) - (pNewInst + hs.length));
            
            // Complete the function if JMP (FF /4).
            if (hs.opcode == 0xFF && hs.modrm_reg == 4)
                Finished = TRUE;

        }
        else if (DIRECT_RELATIVE_CALL_OPCODE(hs.opcode))
        {
            /*
                Direct relative CALL instruction
                Matches exactly E8 (CALL rel32)
            */

            ULONG_PTR Dest = pOldInst + hs.length + (INT32)hs.imm.imm32;
            Call.address = Dest;
            pCopySrc = &Call;
            CopySize = sizeof(Call);
        }
        else if (DIRECT_RELATIVE_JMP_OPCODE(hs.opcode))
        {
            /*
                Covers direct JMP instructions:
                E9: JMP rel32 (near jump)
                EB: JMP rel8 (short jump)
            */

            ULONG_PTR Dest = pOldInst + hs.length;

            if (hs.opcode == 0xEB) // is short jmp
                Dest += (INT8)hs.imm.imm8;
            else
                Dest += (INT32)hs.imm.imm32;

            // Simply copy an internal jump.
            if (
                (ULONG_PTR)ct->pTarget <= Dest  /* Destination is higher the target function address */
                && Dest < ((ULONG_PTR)ct->pTarget + sizeof(JMP_REL))) /* Destination is lower then the end of the REL JUMP so its inside the stolen bytes */
            {
                // No need to adjust
                if (JmpDest < Dest) /* Update the Max Jump Dest we need to grab*/
                    JmpDest = Dest;
            }
            else
            {
                Jmp.address = Dest;
                pCopySrc = &Jmp;
                CopySize = sizeof(Jmp);

                // Finish if we didnt promise to grab future instruction for internal jumps
                Finished = (pOldInst >= JmpDest);
            }

        }
        else if (DIRECT_RELATIVE_LOOP_COND_OPCODE(hs.opcode) ||
                DIRECT_RELATIVE_LOOP_COND_OPCODE2(hs.opcode2))
        {
            /* 
                Comprehensive check for all conditional branches and loops:
                70-7F: Jcc rel8 (short conditional jumps)
                E0-E3: LOOP/LOOPE/LOOPNE/JCXZ rel8
                0F 80-8F: Jcc rel32 (near conditional jumps)
            */

              ULONG_PTR Dest = pOldInst + hs.length;

              if ((hs.opcode & 0xF0) == 0x70      // Jcc
                  || (hs.opcode & 0xFC) == 0xE0)  // LOOPNZ/LOOPZ/LOOP/JECXZ
                  Dest += (INT8)hs.imm.imm8;
              else
                  Dest += (INT32)hs.imm.imm32;

              // Simply copy an internal jump.
              if ((ULONG_PTR)ct->pTarget <= Dest
                  && Dest < ((ULONG_PTR)ct->pTarget + sizeof(JMP_REL)))
              {
                  if (JmpDest < Dest)
                      JmpDest = Dest;
              }
              else if ((hs.opcode & 0xFC) == 0xE0)
              {
                  // LOOPNZ/LOOPZ/LOOP/JCXZ/JECXZ to the outside are not supported.
                  return FALSE;
              }
              else
              {
                  UINT8 cond = ((hs.opcode != 0x0F ? hs.opcode : hs.opcode2) & 0x0F);
                  // Invert the condition in x64 mode to simplify the conditional jump logic.
                  Jcc.opcode = 0x71 ^ cond;
                  Jcc.address = Dest;
                  pCopySrc = &Jcc;
                  CopySize = sizeof(Jcc);
              }

        }
        else if (RET_OPCODE(hs.opcode))
        {
            /*
                Detects RET instructions:
                C2: RET imm16 (return with stack adjustment)
                C3: RET (simple return)
                Not RIP-relative but critical for control flow tracing
            */

            // Complete the function if not in a branch.
            Finished = (pOldInst >= JmpDest);
        }

        // Failed altering the instruction
        if (pOldInst < JmpDest && CopySize != hs.length)
            return FALSE;

        // Trampoline function is too large
        if ((NewPos + CopySize) > TRAMPOLINE_MAX_SIZE)
            return FALSE;

        // Trampoline function has too many instructions
        if (ct->nIP >= ARRAYSIZE(ct->oldIPs))
            return FALSE;

        // Update instruction mapping table
        ct->oldIPs[ct->nIP] = OldPos;
        ct->newIPs[ct->nIP] = NewPos;
        ct->nIP++;

        CopyMemory(LPBYTE(ct->pTrampoline) + NewPos, (LPBYTE)pCopySrc, CopySize);

        NewPos += CopySize;
        OldPos += hs.length;

    } while (!Finished);

 
  
    // Check if we don't have enough space for a standard 5-byte jump
    BOOL standardJumpImpossible = (OldPos < sizeof(JMP_REL) &&
        !IsCodePadding((LPBYTE)ct->pTarget + OldPos, sizeof(JMP_REL) - OldPos));
    if (standardJumpImpossible)
    {
        // Try short jump - if we can't use a 2-byte jump either, hooking is impossible
        BOOL shortJumpImpossible = (OldPos < sizeof(JMP_REL_SHORT) &&
            !IsCodePadding((LPBYTE)ct->pTarget + OldPos, sizeof(JMP_REL_SHORT) - OldPos));

        if (shortJumpImpossible)
            return FALSE; // Can't hook this function

        // Attempt "patch above" technique - check memory before function
        LPBYTE preTargetMem = (LPBYTE)ct->pTarget - sizeof(JMP_REL);

        // Memory before function must be executable
        if (!IsExecutableAddress(preTargetMem))
            return FALSE;

        // Memory before function must be code padding
        if (!IsCodePadding(preTargetMem, sizeof(JMP_REL)))
            return FALSE;

        // We can use the "patch above" technique
        ct->HotPathArea = TRUE;
    }


    // Create the relay
    Jmp.address = (ULONG_PTR)ct->pDetour;
    ct->pRelay = (LPBYTE)ct->pTrampoline + NewPos;
    CopyMemory(ct->pRelay, &Jmp, sizeof(Jmp));

    return TRUE;
}


BOOL WINAPI HookCreateHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal)
{
    // Allocate buffer for the trampoline close to the target
    LPVOID pBuffer = GetMemoryBlock(pTarget);

    if (pBuffer == NULL)
        return FALSE;

    TRAMPOLINE ct;

    ct.pTarget = pTarget;
    ct.pDetour = pDetour;
    ct.pTrampoline = pBuffer;

    if (!HookCreateTrampoline(&ct))
        return FALSE;

    *ppOriginal = ct.pTrampoline;

    // Determine patch location and size
    SIZE_T patchSize = sizeof(JMP_REL);
    LPBYTE pPatchTarget = (LPBYTE)pTarget;

    if (ct.HotPathArea) {
        pPatchTarget -= sizeof(JMP_REL);
        patchSize += sizeof(JMP_REL_SHORT);
    }

    // TODO:: Backup for uninstall

    // Change memory protection
    DWORD oldProtect;
    if (!VirtualProtect(pPatchTarget, patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
        return FALSE;

    // Write the jump instruction
    if (ct.HotPathArea) {
        // Write the short jump at the function start
        PJMP_REL_SHORT pShortJmp = (PJMP_REL_SHORT)pTarget;
        pShortJmp->Opcode = 0xEB;  // JMP rel8
        pShortJmp->Operand = (UINT8)(0 - (sizeof(JMP_REL_SHORT) + sizeof(JMP_REL)));

        // Write the long jump before the function
        PJMP_REL pJmp = (PJMP_REL)pPatchTarget;
        pJmp->Opcode = 0xE9;  // JMP rel32

        ULONG_PTR targetAddr = (ULONG_PTR)ct.pRelay;
        ULONG_PTR sourceAddr = (ULONG_PTR)pPatchTarget + sizeof(JMP_REL);
        INT32 offset = (INT32)(targetAddr - sourceAddr);
        pJmp->Operand = offset;
        char dbgMsg[100];
        sprintf_s(dbgMsg, "Jump from 0x%p to 0x%p, offset: 0x%08X",
            (void*)pPatchTarget, (void*)ct.pRelay, offset);
        OutputDebugStringA(dbgMsg);
        char dbgMsg1[100];
        sprintf_s(dbgMsg1, "Relay at: 0x%p, calculated jump target: 0x%p",
            ct.pRelay, (void*)((ULONG_PTR)pPatchTarget + sizeof(JMP_REL) + offset));
        OutputDebugStringA(dbgMsg);
    }
    else {
        // Standard patching with a single jump
        PJMP_REL pJmp = (PJMP_REL)pPatchTarget;
        pJmp->Opcode = 0xE9;  // JMP rel32
        ULONG_PTR targetAddr = (ULONG_PTR)ct.pRelay;
        ULONG_PTR sourceAddr = (ULONG_PTR)pPatchTarget + sizeof(JMP_REL);
        INT32 offset = (INT32)(targetAddr - sourceAddr);
        pJmp->Operand = offset;
        char dbgMsg[100];
        sprintf_s(dbgMsg, "Jump from 0x%p to 0x%p, offset: 0x%08X",
            (void*)pPatchTarget, (void*)ct.pRelay, offset);
        OutputDebugStringA(dbgMsg);
        char dbgMsg1[100];
        sprintf_s(dbgMsg1, "Relay at: 0x%p, calculated jump target: 0x%p",
            ct.pRelay, (void*)((ULONG_PTR)pPatchTarget + sizeof(JMP_REL) + offset));
        OutputDebugStringA(dbgMsg);
    }

    // Restore memory protection
    VirtualProtect(pPatchTarget, patchSize, oldProtect, &oldProtect);

    // Return pointer to the trampoline (original function)
    return TRUE;
}