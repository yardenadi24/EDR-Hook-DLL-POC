
#include "x86_opcode_defs.h"  /* For all the constants and definitions */
/*
 * Group opcode tables
 * These tables define the characteristics of instructions that use the ModR/M.reg field
 * to select a specific instruction within a group.
 */

const group_table_entry_t x86_group_opcode_table[] = {
    /* Group 1 (0x80-0x83): ADD, OR, ADC, SBB, AND, SUB, XOR, CMP r/m, imm */
    {
        0x00,           /* modrm (Any ModR/M value - used only for identification) */
        GROUP_1,        /* index */
        {               /* entries */
        /* 000: ADD */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 001: OR  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 010: ADC */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 011: SBB */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 100: AND */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 101: SUB */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 110: XOR */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 111: CMP */ { C_NONE, PRE_ANY, F_NONE }
    }
},

/* Group 2 (0xC0, 0xC1, 0xD0-0xD3): ROL, ROR, RCL, RCR, SHL/SAL, SHR, SAR */
{
    0x00,           /* modrm */
    GROUP_2,        /* index */
    {               /* entries */
        /* 000: ROL */ { C_NONE, PRE_ANY, F_NONE },
        /* 001: ROR */ { C_NONE, PRE_ANY, F_NONE },
        /* 010: RCL */ { C_NONE, PRE_ANY, F_NONE },
        /* 011: RCR */ { C_NONE, PRE_ANY, F_NONE },
        /* 100: SHL/SAL */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 101: SHR */ { C_NONE, PRE_ANY, F_NONE },
        /* 110: SHL/SAL */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },  /* Same as entry 4 */
        /* 111: SAR */ { C_NONE, PRE_ANY, F_NONE }
    }
},

/* Group 3 (0xF6, 0xF7): TEST, TEST, NOT, NEG, MUL, IMUL, DIV, IDIV */
{
    0x00,           /* modrm */
    GROUP_3,        /* index */
    {               /* entries */
        /* 000: TEST */ { C_IMM_P66, PRE_ANY, F_NONE }, /* F6: imm8, F7: imm16/32 */
        /* 001: TEST */ { C_IMM_P66, PRE_ANY, F_NONE }, /* F6: imm8, F7: imm16/32 (duplicate) */
        /* 010: NOT  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 011: NEG  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 100: MUL  */ { C_NONE, PRE_ANY, F_NONE },
        /* 101: IMUL */ { C_NONE, PRE_ANY, F_NONE },
        /* 110: DIV  */ { C_NONE, PRE_ANY, F_NONE },
        /* 111: IDIV */ { C_NONE, PRE_ANY, F_NONE }
    }
},

/* Group 4 (0xFE): INC, DEC */
{
    0x00,           /* modrm */
    GROUP_4,        /* index */
    {               /* entries */
        /* 000: INC  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 001: DEC  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 010: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 011: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: ???  */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 5 (0xFF): INC, DEC, CALL, CALL/CALLF, JMP, JMP/JMPF, PUSH */
{
    0x00,           /* modrm */
    GROUP_5,        /* index */
    {               /* entries */
        /* 000: INC   */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 001: DEC   */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 010: CALL  */ { C_NONE, PRE_ANY, F_NONE },
        /* 011: CALLF */ { C_NONE, PRE_ANY, F_NONE },
        /* 100: JMP   */ { C_NONE, PRE_ANY, F_NONE },
        /* 101: JMPF  */ { C_NONE, PRE_ANY, F_NONE },
        /* 110: PUSH  */ { C_NONE, PRE_ANY, F_NONE },
        /* 111: ???   */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 6 (0F 00): SLDT, STR, LLDT, LTR, VERR, VERW */
{
    0x00,           /* modrm */
    GROUP_6,        /* index */
    {               /* entries */
        /* 000: SLDT */ { C_NONE, PRE_ANY, F_NONE },
        /* 001: STR  */ { C_NONE, PRE_ANY, F_NONE },
        /* 010: LLDT */ { C_NONE, PRE_NONE, F_NONE },
        /* 011: LTR  */ { C_NONE, PRE_NONE, F_NONE },
        /* 100: VERR */ { C_NONE, PRE_ANY, F_NONE },
        /* 101: VERW */ { C_NONE, PRE_ANY, F_NONE },
        /* 110: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: ???  */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 7 (0F 01): SGDT, SIDT, LGDT, LIDT, SMSW, LMSW, INVLPG, SWAPGS */
{
    0x00,           /* modrm */
    GROUP_7,        /* index */
    {               /* entries */
        /* 000: SGDT/VMCALL/MONITOR     */ { C_NONE, PRE_ANY, F_SPECIAL },
        /* 001: SIDT/VMLAUNCH/MWAIT     */ { C_NONE, PRE_ANY, F_SPECIAL },
        /* 010: LGDT/VMRESUME/CLAC      */ { C_NONE, PRE_ANY, F_SPECIAL },
        /* 011: LIDT/VMXOFF/STAC        */ { C_NONE, PRE_ANY, F_SPECIAL },
        /* 100: SMSW                    */ { C_NONE, PRE_ANY, F_NONE },
        /* 101: ???                     */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: LMSW                    */ { C_NONE, PRE_NONE, F_NONE },
        /* 111: INVLPG/SWAPGS/RDTSCP    */ { C_NONE, PRE_ANY, F_SPECIAL }
    }
},

/* Group 8 (0F BA): BT, BTS, BTR, BTC */
{
    0x00,           /* modrm */
    GROUP_8,        /* index */
    {               /* entries */
        /* 000: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 001: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 010: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 011: ???  */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: BT   */ { C_NONE, PRE_ANY, F_NONE },
        /* 101: BTS  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 110: BTR  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 111: BTC  */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE }
    }
},

/* Group 9 (0F C7): CMPXCHG8B/CMPXCHG16B, VMPTRLD, VMCLEAR, VMXON */
{
    0x00,           /* modrm */
    GROUP_9,        /* index */
    {               /* entries */
        /* 000: ???          */ { C_ERROR, PRE_NONE, F_NONE },
        /* 001: CMPXCHG8B/16B */ { C_NONE, PRE_ANY | PRE_LOCK, F_NONE },
        /* 010: ???          */ { C_ERROR, PRE_NONE, F_NONE },
        /* 011: ???          */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: ???          */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ???          */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: VMPTRLD/VMCLEAR */ { C_NONE, PREFIX_OPERAND_SIZE, F_SPECIAL },
        /* 111: VMXON        */ { C_NONE, PREFIX_REP, F_NONE }
    }
},

/* Group 10 (0F B9): UD2 */
{
    0x00,           /* modrm */
    GROUP_10,       /* index */
    {               /* entries */
        /* 000: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 001: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 010: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 011: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: ??? */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 11 (C6, C7): MOV */
{
    0x00,           /* modrm */
    GROUP_11,       /* index */
    {               /* entries */
        /* 000: MOV */ { C_NONE, PRE_ANY, F_NONE },
        /* 001: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 010: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 011: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: ??? */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 12 (0F 71): PSRLW, PSRAW, PSLLW */
{
    0x00,           /* modrm */
    GROUP_12,       /* index */
    {               /* entries */
        /* 000: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 001: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 010: PSRLW */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 011: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: PSRAW */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: PSLLW */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 111: ??? */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 13 (0F 72): PSRLD, PSRAD, PSLLD */
{
    0x00,           /* modrm */
    GROUP_13,       /* index */
    {               /* entries */
        /* 000: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 001: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 010: PSRLD */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 011: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: PSRAD */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: PSLLD */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 111: ??? */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 14 (0F 73): PSRLQ, PSRLDQ, PSLLQ, PSLLDQ */
{
    0x00,           /* modrm */
    GROUP_14,       /* index */
    {               /* entries */
        /* 000: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 001: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 010: PSRLQ */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 011: PSRLDQ */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 100: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: PSLLQ */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },
        /* 111: PSLLDQ */ { C_IMM8, PREFIX_OPERAND_SIZE, F_NONE }
    }
},

/* Group 15 (0F AE): FXSAVE, FXRSTOR, LDMXCSR, STMXCSR, XSAVE, XRSTOR, CLFLUSH */
{
    0x00,           /* modrm */
    GROUP_15,       /* index */
    {               /* entries */
        /* 000: FXSAVE */ { C_NONE, PRE_ANY, F_NONE },
        /* 001: FXRSTOR */ { C_NONE, PRE_ANY, F_NONE },
        /* 010: LDMXCSR */ { C_NONE, PRE_ANY, F_NONE },
        /* 011: STMXCSR */ { C_NONE, PRE_ANY, F_NONE },
        /* 100: XSAVE */ { C_NONE, PRE_ANY, F_NONE },
        /* 101: XRSTOR */ { C_NONE, PRE_ANY, F_NONE },
        /* 110: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: CLFLUSH */ { C_NONE, PRE_ANY, F_NONE }
    }
},

/* Group 16 (0F 18): PREFETCH */
{
    0x00,           /* modrm */
    GROUP_16,       /* index */
    {               /* entries */
        /* 000: PREFETCHNTA */ { C_NONE, PRE_ANY, F_NONE },
        /* 001: PREFETCHT0 */ { C_NONE, PRE_ANY, F_NONE },
        /* 010: PREFETCHT1 */ { C_NONE, PRE_ANY, F_NONE },
        /* 011: PREFETCHT2 */ { C_NONE, PRE_ANY, F_NONE },
        /* 100: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: ??? */ { C_ERROR, PRE_NONE, F_NONE }
    }
},

/* Group 17 (0F 78): EXTRQ, INSERTQ */
{
    0x00,           /* modrm */
    GROUP_17,       /* index */
    {               /* entries */
        /* 000: EXTRQ */ { C_NONE, PREFIX_OPERAND_SIZE, F_NONE },
        /* 001: INSERTQ */ { C_NONE, PREFIX_REPNE, F_NONE },
        /* 010: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 011: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 100: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 101: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 110: ??? */ { C_ERROR, PRE_NONE, F_NONE },
        /* 111: ??? */ { C_ERROR, PRE_NONE, F_NONE }
    }
}
};

/* Size of the group opcode table */
const int x86_group_opcode_table_size = sizeof(x86_group_opcode_table) / sizeof(group_table_entry_t);
