#pragma once
#include "x86_opcode_defs.h"

/*
 * 1-byte opcode table (00-FF)
 * Each entry describes the characteristics of the corresponding opcode
 */
const opcode_table_entry_t x86_1byte_opcode_table[256] = {
    /* 00 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* ADD r/m8, r8 */
    /* 01 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* ADD r/m16/32/64, r16/32/64 */
    /* 02 */ { C_MODRM, PRE_ANY, F_NONE },             /* ADD r8, r/m8 */
    /* 03 */ { C_MODRM, PRE_ANY, F_NONE },             /* ADD r16/32/64, r/m16/32/64 */
    /* 04 */ { C_IMM8, PRE_ANY, F_NONE },              /* ADD AL, imm8 */
    /* 05 */ { C_IMM_P66, PRE_ANY, F_NONE },           /* ADD AX/EAX/RAX, imm16/32 */
    /* 06 */ { C_ERROR, PRE_NONE, F_NONE },            /* PUSH ES (invalid in 64-bit mode) */
    /* 07 */ { C_ERROR, PRE_NONE, F_NONE },            /* POP ES (invalid in 64-bit mode) */
    /* 08 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* OR r/m8, r8 */
    /* 09 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* OR r/m16/32/64, r16/32/64 */
    /* 0A */ { C_MODRM, PRE_ANY, F_NONE },             /* OR r8, r/m8 */
    /* 0B */ { C_MODRM, PRE_ANY, F_NONE },             /* OR r16/32/64, r/m16/32/64 */
    /* 0C */ { C_IMM8, PRE_ANY, F_NONE },              /* OR AL, imm8 */
    /* 0D */ { C_IMM_P66, PRE_ANY, F_NONE },           /* OR AX/EAX/RAX, imm16/32 */
    /* 0E */ { C_ERROR, PRE_NONE, F_NONE },            /* PUSH CS (invalid in 64-bit mode) */
    /* 0F */ { C_NONE, PRE_ANY, F_SPECIAL },           /* Two-byte opcode escape */

    /* 10 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* ADC r/m8, r8 */
    /* 11 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* ADC r/m16/32/64, r16/32/64 */
    /* 12 */ { C_MODRM, PRE_ANY, F_NONE },             /* ADC r8, r/m8 */
    /* 13 */ { C_MODRM, PRE_ANY, F_NONE },             /* ADC r16/32/64, r/m16/32/64 */
    /* 14 */ { C_IMM8, PRE_ANY, F_NONE },              /* ADC AL, imm8 */
    /* 15 */ { C_IMM_P66, PRE_ANY, F_NONE },           /* ADC AX/EAX/RAX, imm16/32 */
    /* 16 */ { C_ERROR, PRE_NONE, F_NONE },            /* PUSH SS (invalid in 64-bit mode) */
    /* 17 */ { C_ERROR, PRE_NONE, F_NONE },            /* POP SS (invalid in 64-bit mode) */
    /* 18 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* SBB r/m8, r8 */
    /* 19 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* SBB r/m16/32/64, r16/32/64 */
    /* 1A */ { C_MODRM, PRE_ANY, F_NONE },             /* SBB r8, r/m8 */
    /* 1B */ { C_MODRM, PRE_ANY, F_NONE },             /* SBB r16/32/64, r/m16/32/64 */
    /* 1C */ { C_IMM8, PRE_ANY, F_NONE },              /* SBB AL, imm8 */
    /* 1D */ { C_IMM_P66, PRE_ANY, F_NONE },           /* SBB AX/EAX/RAX, imm16/32 */
    /* 1E */ { C_ERROR, PRE_NONE, F_NONE },            /* PUSH DS (invalid in 64-bit mode) */
    /* 1F */ { C_ERROR, PRE_NONE, F_NONE },            /* POP DS (invalid in 64-bit mode) */

    /* 20 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* AND r/m8, r8 */
    /* 21 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* AND r/m16/32/64, r16/32/64 */
    /* 22 */ { C_MODRM, PRE_ANY, F_NONE },             /* AND r8, r/m8 */
    /* 23 */ { C_MODRM, PRE_ANY, F_NONE },             /* AND r16/32/64, r/m16/32/64 */
    /* 24 */ { C_IMM8, PRE_ANY, F_NONE },              /* AND AL, imm8 */
    /* 25 */ { C_IMM_P66, PRE_ANY, F_NONE },           /* AND AX/EAX/RAX, imm16/32 */
    /* 26 */ { C_NONE, PRE_ANY, F_SPECIAL },            /* ES segment override (prefix) */
    /* 27 */ { C_ERROR, PRE_NONE, F_NONE },            /* DAA (invalid in 64-bit mode) */
    /* 28 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* SUB r/m8, r8 */
    /* 29 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* SUB r/m16/32/64, r16/32/64 */
    /* 2A */ { C_MODRM, PRE_ANY, F_NONE },             /* SUB r8, r/m8 */
    /* 2B */ { C_MODRM, PRE_ANY, F_NONE },             /* SUB r16/32/64, r/m16/32/64 */
    /* 2C */ { C_IMM8, PRE_ANY, F_NONE },              /* SUB AL, imm8 */
    /* 2D */ { C_IMM_P66, PRE_ANY, F_NONE },           /* SUB AX/EAX/RAX, imm16/32 */
    /* 2E */ { C_NONE, PRE_ANY, F_SPECIAL },            /* CS segment override (prefix) */
    /* 2F */ { C_ERROR, PRE_NONE, F_NONE },            /* DAS (invalid in 64-bit mode) */

    /* 30 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* XOR r/m8, r8 */
    /* 31 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* XOR r/m16/32/64, r16/32/64 */
    /* 32 */ { C_MODRM, PRE_ANY, F_NONE },             /* XOR r8, r/m8 */
    /* 33 */ { C_MODRM, PRE_ANY, F_NONE },             /* XOR r16/32/64, r/m16/32/64 */
    /* 34 */ { C_IMM8, PRE_ANY, F_NONE },              /* XOR AL, imm8 */
    /* 35 */ { C_IMM_P66, PRE_ANY, F_NONE },           /* XOR AX/EAX/RAX, imm16/32 */
    /* 36 */ { C_NONE, PRE_ANY, F_SPECIAL },            /* SS segment override (prefix) */
    /* 37 */ { C_ERROR, PRE_NONE, F_NONE },            /* AAA (invalid in 64-bit mode) */
    /* 38 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMP r/m8, r8 */
    /* 39 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMP r/m16/32/64, r16/32/64 */
    /* 3A */ { C_MODRM, PRE_ANY, F_NONE },             /* CMP r8, r/m8 */
    /* 3B */ { C_MODRM, PRE_ANY, F_NONE },             /* CMP r16/32/64, r/m16/32/64 */
    /* 3C */ { C_IMM8, PRE_ANY, F_NONE },              /* CMP AL, imm8 */
    /* 3D */ { C_IMM_P66, PRE_ANY, F_NONE },           /* CMP AX/EAX/RAX, imm16/32 */
    /* 3E */ { C_NONE, PRE_ANY, F_SPECIAL },            /* DS segment override (prefix) */
    /* 3F */ { C_ERROR, PRE_NONE, F_NONE },            /* AAS (invalid in 64-bit mode) */

    /* 40-4F */
    /* These are REX prefixes in 64-bit mode, otherwise INC/DEC r16/32 */
    /* 40 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX prefix / INC (invalid in 64-bit) */
    /* 41 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.B prefix / INC (invalid in 64-bit) */
    /* 42 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.X prefix / INC (invalid in 64-bit) */
    /* 43 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.XB prefix / INC (invalid in 64-bit) */
    /* 44 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.R prefix / INC (invalid in 64-bit) */
    /* 45 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.RB prefix / INC (invalid in 64-bit) */
    /* 46 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.RX prefix / INC (invalid in 64-bit) */
    /* 47 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.RXB prefix / INC (invalid in 64-bit) */
    /* 48 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.W prefix / DEC (invalid in 64-bit) */
    /* 49 */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WB prefix / DEC (invalid in 64-bit) */
    /* 4A */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WX prefix / DEC (invalid in 64-bit) */
    /* 4B */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WXB prefix / DEC (invalid in 64-bit) */
    /* 4C */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WR prefix / DEC (invalid in 64-bit) */
    /* 4D */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WRB prefix / DEC (invalid in 64-bit) */
    /* 4E */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WRX prefix / DEC (invalid in 64-bit) */
    /* 4F */ { C_ERROR, PRE_NONE, F_NONE },            /* REX.WRXB prefix / DEC (invalid in 64-bit) */

    /* 50-57: PUSH r16/32/64 */
    /* 50 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rAX/r8 */
    /* 51 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rCX/r9 */
    /* 52 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rDX/r10 */
    /* 53 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rBX/r11 */
    /* 54 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rSP/r12 */
    /* 55 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rBP/r13 */
    /* 56 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rSI/r14 */
    /* 57 */ { C_NONE, PRE_ANY, F_64MODE },            /* PUSH rDI/r15 */

    /* 58-5F: POP r16/32/64 */
    /* 58 */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rAX/r8 */
    /* 59 */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rCX/r9 */
    /* 5A */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rDX/r10 */
    /* 5B */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rBX/r11 */
    /* 5C */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rSP/r12 */
    /* 5D */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rBP/r13 */
    /* 5E */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rSI/r14 */
    /* 5F */ { C_NONE, PRE_ANY, F_64MODE },            /* POP rDI/r15 */

    /* 60-61: PUSHA/POPA - Invalid in 64-bit mode */
    /* 60 */ { C_ERROR, PRE_NONE, F_NONE },            /* PUSHAD/PUSHAW (invalid in 64-bit) */
    /* 61 */ { C_ERROR, PRE_NONE, F_NONE },            /* POPAD/POPAW (invalid in 64-bit) */

    /* 62 */ { C_ERROR, PRE_NONE, F_NONE },            /* BOUND (invalid in 64-bit) */
    /* 63 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVSXD r64, r/m32 (in 64-bit) */

    /* 64-65: FS/GS segment override prefixes */
    /* 64 */{ C_NONE, PRE_ANY, F_SPECIAL },            /* FS segment override prefix */
    /* 65 */{ C_NONE, PRE_ANY, F_SPECIAL },            /* GS segment override prefix */

    /* 66-67: Operand/Address size override prefixes */
    /* 66 */ { C_NONE, PRE_ANY, F_SPECIAL },           /* Operand size override prefix */
    /* 67 */ { C_NONE, PRE_ANY, F_SPECIAL },           /* Address size override prefix */

    /* 68-6A: PUSH imm */
    /* 68 */ { C_IMM_P66, PRE_ANY, F_NONE },           /* PUSH imm16/32 */
    /* 69 */ { C_MODRM | C_IMM_P66, PRE_ANY, F_NONE }, /* IMUL r16/32/64, r/m16/32/64, imm16/32 */
    /* 6A */ { C_IMM8, PRE_ANY, F_NONE },              /* PUSH imm8 */
    /* 6B */ { C_MODRM | C_IMM8, PRE_ANY, F_NONE },    /* IMUL r16/32/64, r/m16/32/64, imm8 */

    /* 6C-6D: INS/INSB/INSW/INSD */
    /* 6C */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* INS m8, DX / INSB */
    /* 6D */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* INS m16/32, DX / INSW/INSD */

    /* 6E-6F: OUTS/OUTSB/OUTSW/OUTSD */
    /* 6E */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* OUTS DX, m8 / OUTSB */
    /* 6F */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* OUTS DX, m16/32 / OUTSW/OUTSD */

    /* 70-7F: Jcc rel8 */
    /* 70 */ { C_REL8, PRE_ANY, F_NONE },              /* JO rel8 */
    /* 71 */ { C_REL8, PRE_ANY, F_NONE },              /* JNO rel8 */
    /* 72 */ { C_REL8, PRE_ANY, F_NONE },              /* JB/JNAE/JC rel8 */
    /* 73 */ { C_REL8, PRE_ANY, F_NONE },              /* JNB/JAE/JNC rel8 */
    /* 74 */ { C_REL8, PRE_ANY, F_NONE },              /* JZ/JE rel8 */
    /* 75 */ { C_REL8, PRE_ANY, F_NONE },              /* JNZ/JNE rel8 */
    /* 76 */ { C_REL8, PRE_ANY, F_NONE },              /* JBE/JNA rel8 */
    /* 77 */ { C_REL8, PRE_ANY, F_NONE },              /* JNBE/JA rel8 */
    /* 78 */ { C_REL8, PRE_ANY, F_NONE },              /* JS rel8 */
    /* 79 */ { C_REL8, PRE_ANY, F_NONE },              /* JNS rel8 */
    /* 7A */ { C_REL8, PRE_ANY, F_NONE },              /* JP/JPE rel8 */
    /* 7B */ { C_REL8, PRE_ANY, F_NONE },              /* JNP/JPO rel8 */
    /* 7C */ { C_REL8, PRE_ANY, F_NONE },              /* JL/JNGE rel8 */
    /* 7D */ { C_REL8, PRE_ANY, F_NONE },              /* JNL/JGE rel8 */
    /* 7E */ { C_REL8, PRE_ANY, F_NONE },              /* JLE/JNG rel8 */
    /* 7F */ { C_REL8, PRE_ANY, F_NONE },              /* JNLE/JG rel8 */

    /* 80-83: Group 1 instructions with immediate operands */
    /* 80 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_1 },   /* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m8, imm8 */
    /* 81 */ { C_MODRM | C_IMM_P66 | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_1 },/* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m16/32/64, imm16/32 */
    /* 82 */ { C_ERROR, PRE_NONE, F_NONE },                                  /* Invalid in 64-bit mode (alias for 80) */
    /* 83 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_1 },   /* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m16/32/64, imm8 */

    /* 84-85: TEST r/m, r */
    /* 84 */ { C_MODRM, PRE_ANY, F_NONE },             /* TEST r/m8, r8 */
    /* 85 */ { C_MODRM, PRE_ANY, F_NONE },             /* TEST r/m16/32/64, r16/32/64 */

    /* 86-87: XCHG r, r/m */
    /* 86 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* XCHG r8, r/m8 */
    /* 87 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* XCHG r16/32/64, r/m16/32/64 */

    /* 88-8B: MOV instructions */
    /* 88 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOV r/m8, r8 */
    /* 89 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOV r/m16/32/64, r16/32/64 */
    /* 8A */ { C_MODRM, PRE_ANY, F_NONE },             /* MOV r8, r/m8 */
    /* 8B */ { C_MODRM, PRE_ANY, F_NONE },             /* MOV r16/32/64, r/m16/32/64 */

    /* 8C-8E: MOV with segment registers */
    /* 8C */ { C_MODRM, PRE_ANY, F_NONE },             /* MOV r/m16/64, Sreg */
    /* 8D */ { C_MODRM, PRE_ANY, F_NONE },             /* LEA r16/32/64, m */
    /* 8E */ { C_MODRM, PRE_ANY, F_NONE },             /* MOV Sreg, r/m16 */

    /* 8F: POP r/m16/32/64 (Group 1A) */
    /* 8F */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_10 }, /* POP r/m16/32/64 */

    /* 90-97: XCHG AX/EAX/RAX, r16/32/64 */
    /* 90 */ { C_NONE, PRE_ANY, F_SPECIAL },              /* NOP / XCHG rAX, rAX */
    /* 91 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rCX */
    /* 92 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rDX */
    /* 93 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rBX */
    /* 94 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rSP */
    /* 95 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rBP */
    /* 96 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rSI */
    /* 97 */ { C_NONE, PRE_ANY, F_NONE },              /* XCHG rAX, rDI */

    /* 98-9F: Conversion instructions */
    /* 98 */ { C_NONE, PRE_ANY, F_NONE },              /* CBW/CWDE/CDQE */
    /* 99 */ { C_NONE, PRE_ANY, F_NONE },              /* CWD/CDQ/CQO */
    /* 9A */ { C_ERROR, PRE_NONE, F_NONE },            /* CALLF ptr16:16/32 (invalid in 64-bit) */
    /* 9B */ { C_NONE, PRE_ANY, F_NONE },              /* FWAIT/WAIT */
    /* 9C */ { C_NONE, PRE_ANY, F_NONE },              /* PUSHF/PUSHFD/PUSHFQ */
    /* 9D */ { C_NONE, PRE_ANY, F_NONE },              /* POPF/POPFD/POPFQ */
    /* 9E */ { C_NONE, PRE_ANY, F_NONE },              /* SAHF */
    /* 9F */ { C_NONE, PRE_ANY, F_NONE },              /* LAHF */

    /* A0-A3: MOV AL/AX/EAX/RAX <-> moffs */
    /* A0 */{ C_IMM_P66, PRE_ANY, F_SPECIAL },         /* MOV AL, moffs8 */
    /* A1 */{ C_IMM_P66, PRE_ANY, F_SPECIAL },         /* MOV AX/EAX/RAX, moffs16/32/64 */
    /* A2 */{ C_IMM_P66, PRE_ANY, F_SPECIAL },         /* MOV moffs8, AL */
    /* A3 */{ C_IMM_P66, PRE_ANY, F_SPECIAL },         /* MOV moffs16/32/64, AX/EAX/RAX */

    /* A4-A7: String operations (movs, cmps) */
    /* A4 */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* MOVSB */
    /* A5 */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* MOVSW/MOVSD/MOVSQ */
    /* A6 */ { C_NONE, PRE_ANY | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CMPSB */
    /* A7 */ { C_NONE, PRE_ANY | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CMPSW/CMPSD/CMPSQ */

    /* A8-A9: TEST AL/AX/EAX/RAX, imm */
    /* A8 */ { C_IMM8, PRE_ANY, F_NONE },              /* TEST AL, imm8 */
    /* A9 */ { C_IMM_P66, PRE_ANY, F_NONE },           /* TEST AX/EAX/RAX, imm16/32 */

    /* AA-AF: String operations (stos, lods, scas) */
    /* AA */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* STOSB */
    /* AB */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* STOSW/STOSD/STOSQ */
    /* AC */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* LODSB */
    /* AD */ { C_NONE, PRE_ANY | PREFIX_REP, F_NONE },     /* LODSW/LODSD/LODSQ */
    /* AE */ { C_NONE, PRE_ANY | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* SCASB */
    /* AF */ { C_NONE, PRE_ANY | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* SCASW/SCASD/SCASQ */

    /* B0-B7: MOV r8, imm8 */
    /* B0 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV AL/R8L, imm8 */
    /* B1 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV CL/R9L, imm8 */
    /* B2 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV DL/R10L, imm8 */
    /* B3 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV BL/R11L, imm8 */
    /* B4 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV AH/R12L, imm8 */
    /* B5 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV CH/R13L, imm8 */
    /* B6 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV DH/R14L, imm8 */
    /* B7 */ { C_IMM8, PRE_ANY, F_NONE },              /* MOV BH/R15L, imm8 */

    /* B8-BF: MOV r16/32/64, imm16/32/64 */
    /* B8 */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rAX, imm16/32/64 */
    /* B9 */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rCX, imm16/32/64 */
    /* BA */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rDX, imm16/32/64 */
    /* BB */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rBX, imm16/32/64 */
    /* BC */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rSP, imm16/32/64 */
    /* BD */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rBP, imm16/32/64 */
    /* BE */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rSI, imm16/32/64 */
    /* BF */ { C_IMM_P66, PRE_ANY, F_REX_W },          /* MOV rDI, imm16/32/64 */

    /* C0-C1: Group 2 shift instructions with imm8 */
    /* C0 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY, GROUP_2 }, /* ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m8, imm8 */
    /* C1 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY, GROUP_2 }, /* ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m16/32/64, imm8 */

    /* C2-C3: RET instructions */
    /* C2 */ { C_IMM16, PRE_NONE, F_NONE },            /* RET imm16 */
    /* C3 */ { C_NONE, PRE_NONE, F_NONE },             /* RET */

    /* C4-C5: VEX prefixes in 64-bit mode, LES/LDS in 32-bit mode */
    /* C4 */ { C_ERROR, PRE_NONE, F_NONE },            /* VEX3 prefix / LES (invalid in 64-bit) */
    /* C5 */ { C_ERROR, PRE_NONE, F_NONE },            /* VEX2 prefix / LDS (invalid in 64-bit) */

    /* C6-C7: MOV r/m, imm (Group 11) */
    /* C6 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY, GROUP_11 },   /* MOV r/m8, imm8 */
    /* C7 */ { C_MODRM | C_IMM_P66 | C_GROUP, PRE_ANY, GROUP_11 },/* MOV r/m16/32/64, imm16/32 */

    /* C8-C9: ENTER/LEAVE */
    /* C8 */ { C_IMM16 | C_IMM8, PRE_NONE, F_NONE },   /* ENTER imm16, imm8 */
    /* C9 */ { C_NONE, PRE_NONE, F_NONE },             /* LEAVE */

    /* CA-CB: Far RET */
    /* CA */ { C_IMM16, PRE_NONE, F_NONE },            /* RETF imm16 */
    /* CB */ { C_NONE, PRE_NONE, F_NONE },             /* RETF */

    /* CC-CE: INT instructions */
    /* CC */ { C_NONE, PRE_NONE, F_NONE },             /* INT 3 */
    /* CD */ { C_IMM8, PRE_NONE, F_NONE },             /* INT imm8 */
    /* CE */ { C_ERROR, PRE_NONE, F_NONE },            /* INTO (invalid in 64-bit) */

    /* CF: IRET */
    /* CF */ { C_NONE, PRE_NONE, F_NONE },             /* IRETD/IRETQ */

    /* D0-D3: Group 2 shift instructions */
    /* D0 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_2 },  /* ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m8, 1 */
    /* D1 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_2 },  /* ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m16/32/64, 1 */
    /* D2 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_2 },  /* ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m8, CL */
    /* D3 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_2 },  /* ROL/ROR/RCL/RCR/SHL/SHR/SAL/SAR r/m16/32/64, CL */

    /* D4-D5: AAM/AAD - Invalid in 64-bit mode */
    /* D4 */ { C_ERROR, PRE_NONE, F_NONE },            /* AAM (invalid in 64-bit) */
    /* D5 */ { C_ERROR, PRE_NONE, F_NONE },            /* AAD (invalid in 64-bit) */

    /* D6: SALC - Invalid in 64-bit mode */
    /* D6 */ { C_ERROR, PRE_NONE, F_NONE },            /* SALC (invalid in 64-bit) */

    /* D7: XLAT */
    /* D7 */ { C_NONE, PRE_ANY, F_NONE },              /* XLAT/XLATB */

    /* D8-DF: x87 FPU instructions */
    /* D8 */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* D9 */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* DA */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* DB */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* DC */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* DD */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* DE */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */
    /* DF */ { C_MODRM, PRE_ANY, F_NONE },             /* x87 instructions */

    /* E0-E3: Loop and jump instructions */
    /* E0 */ { C_REL8, PRE_ANY, F_NONE },              /* LOOPNE/LOOPNZ rel8 */
    /* E1 */ { C_REL8, PRE_ANY, F_NONE },              /* LOOPE/LOOPZ rel8 */
    /* E2 */ { C_REL8, PRE_ANY, F_NONE },              /* LOOP rel8 */
    /* E3 */ { C_REL8, PRE_ANY, F_NONE },              /* JCXZ/JECXZ/JRCXZ rel8 */

    /* E4-E7: IN/OUT instructions */
    /* E4 */ { C_IMM8, PRE_NONE, F_NONE },             /* IN AL, imm8 */
    /* E5 */ { C_IMM8, PRE_NONE, F_NONE },             /* IN eAX, imm8 */
    /* E6 */ { C_IMM8, PRE_NONE, F_NONE },             /* OUT imm8, AL */
    /* E7 */ { C_IMM8, PRE_NONE, F_NONE },             /* OUT imm8, eAX */

    /* E8-EB: CALL and JMP instructions */
    /* E8 */ { C_REL32, PRE_NONE, F_NONE },            /* CALL rel32 */
    /* E9 */ { C_REL32, PRE_NONE, F_NONE },            /* JMP rel32 */
    /* EA */ { C_ERROR, PRE_NONE, F_NONE },            /* JMPF ptr16:16/32 (invalid in 64-bit mode) */
    /* EB */ { C_REL8, PRE_NONE, F_NONE },             /* JMP rel8 */

    /* EC-EF: IN/OUT with DX */
    /* EC */ { C_NONE, PRE_NONE, F_NONE },             /* IN AL, DX */
    /* ED */ { C_NONE, PRE_NONE, F_NONE },             /* IN eAX, DX */
    /* EE */ { C_NONE, PRE_NONE, F_NONE },             /* OUT DX, AL */
    /* EF */ { C_NONE, PRE_NONE, F_NONE },             /* OUT DX, eAX */

    /* F0-F3: Lock and repeat prefixes */
    /* F0 */ { C_NONE, PRE_ANY, F_SPECIAL },           /* LOCK prefix */
    /* F1 */ { C_NONE, PRE_NONE, F_NONE },             /* INT1/ICEBP */
    /* F2 */ { C_NONE, PRE_ANY, F_SPECIAL },           /* REPNE/REPNZ prefix */
    /* F3 */ { C_NONE, PRE_ANY, F_SPECIAL },           /* REP/REPE/REPZ prefix */

    /* F4-F5: HALT and CMC */
    /* F4 */ { C_NONE, PRE_NONE, F_NONE },             /* HLT */
    /* F5 */ { C_NONE, PRE_NONE, F_NONE },             /* CMC */

    /* F6-F7: Group 3 unary instructions */
    /* F6 */ { C_MODRM | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_3 }, /* TEST/TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m8 */
    /* F7 */ { C_MODRM | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_3 }, /* TEST/TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m16/32/64 */

    /* F8-FD: Flag manipulation */
    /* F8 */ { C_NONE, PRE_NONE, F_NONE },             /* CLC */
    /* F9 */ { C_NONE, PRE_NONE, F_NONE },             /* STC */
    /* FA */ { C_NONE, PRE_NONE, F_NONE },             /* CLI */
    /* FB */ { C_NONE, PRE_NONE, F_NONE },             /* STI */
    /* FC */ { C_NONE, PRE_NONE, F_NONE },             /* CLD */
    /* FD */ { C_NONE, PRE_NONE, F_NONE },             /* STD */

    /* FE-FF: Group 4, 5 instructions */
    /* FE */ { C_MODRM | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_4 }, /* INC/DEC r/m8 */
    /* FF */ { C_MODRM | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_5 }, /* INC/DEC/CALL/CALLF/JMP/JMPF/PUSH r/m16/32/64 */
};