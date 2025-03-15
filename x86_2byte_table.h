#pragma once

#include "x86_opcode_defs.h"

/*
 * 2-byte opcode table (0F xx)
 * Each entry describes the characteristics of the corresponding opcode
 */
const opcode_table_entry_t x86_2byte_opcode_table[256] = {
    /* 0F 00 - Group 6 */
    /* 00 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_6 },  /* SLDT/STR/LLDT/LTR/VERR/VERW */

    /* 0F 01 - Group 7 */
    /* 01 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_7 },  /* SGDT/SIDT/LGDT/LIDT/SMSW/LMSW/INVLPG/SWAPGS */

    /* 0F 02-03 - LAR, LSL */
    /* 02 */ { C_MODRM, PRE_ANY, F_NONE },             /* LAR r16/32/64, r/m16/32 */
    /* 03 */ { C_MODRM, PRE_ANY, F_NONE },             /* LSL r16/32/64, r/m16/32 */

    /* 0F 04 - INVALID */
    /* 04 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */

    /* 0F 05 - SYSCALL */
    /* 05 */ { C_NONE, PRE_NONE, F_64MODE },           /* SYSCALL */

    /* 0F 06-07 - CLTS, SYSRET */
    /* 06 */ { C_NONE, PRE_NONE, F_NONE },             /* CLTS */
    /* 07 */ { C_NONE, PRE_NONE, F_64MODE },           /* SYSRET */

    /* 0F 08-09 - INVD, WBINVD */
    /* 08 */ { C_NONE, PRE_NONE, F_NONE },             /* INVD */
    /* 09 */ { C_NONE, PRE_NONE, F_NONE },             /* WBINVD */

    /* 0F 0A - INVALID */
    /* 0A */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */

    /* 0F 0B - UD2 (undefined instruction) */
    /* 0B */ { C_NONE, PRE_NONE, F_NONE },             /* UD2 */

    /* 0F 0C-0F - INVALID */
    /* 0C */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 0D */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 0E */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 0F */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */

    /* 0F 10-17 - SSE data transfer */
    /* 10 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* MOVUPS/MOVUPD/MOVSS/MOVSD xmm, xmm/m */
    /* 11 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* MOVUPS/MOVUPD/MOVSS/MOVSD xmm/m, xmm */
    /* 12 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* MOVLPS/MOVLPD/MOVSLDUP/MOVDDUP xmm, xmm/m */
    /* 13 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVLPS m, xmm */
    /* 14 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* UNPCKLPS/UNPCKLPD xmm, xmm/m */
    /* 15 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* UNPCKHPS/UNPCKHPD xmm, xmm/m */
    /* 16 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP, F_NONE }, /* MOVHPS/MOVHPD/MOVSHDUP xmm, xmm/m */
    /* 17 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVHPS/MOVHPD m, xmm */

    /* 0F 18-1F - Prefetch, NOP, CLFLUSH */
    /* 18 */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_16 }, /* PREFETCH (mod != 3) / NOP (mod == 3) */
    /* 19 */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */
    /* 1A */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */
    /* 1B */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */
    /* 1C */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */
    /* 1D */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */
    /* 1E */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */
    /* 1F */ { C_MODRM, PRE_ANY, F_NONE },             /* NOP/HINT r/m16/32 */

    /* 0F 20-23 - MOV to/from control/debug registers */
    /* 20 */ { C_MODRM, PRE_NONE, F_NONE },            /* MOV r64, CR0-CR7 */
    /* 21 */ { C_MODRM, PRE_NONE, F_NONE },            /* MOV r64, DR0-DR7 */
    /* 22 */ { C_MODRM, PRE_NONE, F_NONE },            /* MOV CR0-CR7, r64 */
    /* 23 */ { C_MODRM, PRE_NONE, F_NONE },            /* MOV DR0-DR7, r64 */

    /* 0F 24-27 - INVALID */
    /* 24 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 25 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 26 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 27 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */

    /* 0F 28-2F - SSE data movement */
    /* 28 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVAPS/MOVAPD xmm, xmm/m */
    /* 29 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVAPS/MOVAPD xmm/m, xmm */
    /* 2A */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CVTPI2PS/CVTPI2PD/CVTSI2SS/CVTSI2SD xmm, xmm/m */
    /* 2B */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVNTPS/MOVNTPD m, xmm */
    /* 2C */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CVTTPS2PI/CVTTPD2PI/CVTTSS2SI/CVTTSD2SI xmm, xmm/m */
    /* 2D */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CVTPS2PI/CVTPD2PI/CVTSS2SI/CVTSD2SI xmm, xmm/m */
    /* 2E */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* UCOMISS/UCOMISD xmm, xmm/m */
    /* 2F */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* COMISS/COMISD xmm, xmm/m */

    /* 0F 30-3F - WRMSR, RDTSC, RDMSR, RDPMC, SYSENTER, SYSEXIT, GETSEC, CMOV-series */
    /* 30 */ { C_NONE, PRE_NONE, F_NONE },             /* WRMSR */
    /* 31 */ { C_NONE, PRE_NONE, F_NONE },             /* RDTSC */
    /* 32 */ { C_NONE, PRE_NONE, F_NONE },             /* RDMSR */
    /* 33 */ { C_NONE, PRE_NONE, F_NONE },             /* RDPMC */
    /* 34 */ { C_NONE, PRE_NONE, F_NONE },             /* SYSENTER */
    /* 35 */ { C_NONE, PRE_NONE, F_NONE },             /* SYSEXIT */
    /* 36 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 37 */ { C_NONE, PRE_NONE, F_NONE },             /* GETSEC */
    /* 38 */ { C_NONE, PRE_ANY, F_SPECIAL },           /* Three-byte opcode escape (0F 38) */
    /* 39 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 3A */ { C_NONE, PRE_ANY, F_SPECIAL },           /* Three-byte opcode escape (0F 3A) */
    /* 3B */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 3C */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 3D */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 3E */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 3F */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */

    /* 0F 40-4F - Conditional moves (CMOVcc) */
    /* 40 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVO r16/32/64, r/m16/32/64 */
    /* 41 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVNO r16/32/64, r/m16/32/64 */
    /* 42 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVB/CMOVC/CMOVNAE r16/32/64, r/m16/32/64 */
    /* 43 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVAE/CMOVNB/CMOVNC r16/32/64, r/m16/32/64 */
    /* 44 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVE/CMOVZ r16/32/64, r/m16/32/64 */
    /* 45 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVNE/CMOVNZ r16/32/64, r/m16/32/64 */
    /* 46 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVBE/CMOVNA r16/32/64, r/m16/32/64 */
    /* 47 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVA/CMOVNBE r16/32/64, r/m16/32/64 */
    /* 48 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVS r16/32/64, r/m16/32/64 */
    /* 49 */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVNS r16/32/64, r/m16/32/64 */
    /* 4A */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVP/CMOVPE r16/32/64, r/m16/32/64 */
    /* 4B */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVNP/CMOVPO r16/32/64, r/m16/32/64 */
    /* 4C */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVL/CMOVNGE r16/32/64, r/m16/32/64 */
    /* 4D */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVGE/CMOVNL r16/32/64, r/m16/32/64 */
    /* 4E */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVLE/CMOVNG r16/32/64, r/m16/32/64 */
    /* 4F */ { C_MODRM, PRE_ANY, F_NONE },             /* CMOVG/CMOVNLE r16/32/64, r/m16/32/64 */

    /* 0F 50-5F - SSE, MOVMSKPS/PD, SQRT, RSQ, RCP, AND, OR, XOR, ADD, MUL, CVTT, DIV, MIN, MAX */
    /* 50 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVMSKPS/MOVMSKPD r, xmm */
    /* 51 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* SQRTPS/SQRTPD/SQRTSS/SQRTSD xmm, xmm/m */
    /* 52 */ { C_MODRM, PRE_ANY | PREFIX_REP, F_NONE },    /* RSQRTPS/RSQRTSS xmm, xmm/m */
    /* 53 */ { C_MODRM, PRE_ANY | PREFIX_REP, F_NONE },    /* RCPPS/RCPSS xmm, xmm/m */
    /* 54 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* ANDPS/ANDPD xmm, xmm/m */
    /* 55 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* ANDNPS/ANDNPD xmm, xmm/m */
    /* 56 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* ORPS/ORPD xmm, xmm/m */
    /* 57 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* XORPS/XORPD xmm, xmm/m */
    /* 58 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* ADDPS/ADDPD/ADDSS/ADDSD xmm, xmm/m */
    /* 59 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* MULPS/MULPD/MULSS/MULSD xmm, xmm/m */
    /* 5A */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CVTPS2PD/CVTPD2PS/CVTSS2SD/CVTSD2SS xmm, xmm/m */
    /* 5B */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP, F_NONE }, /* CVTDQ2PS/CVTPS2DQ/CVTTPS2DQ xmm, xmm/m */
    /* 5C */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* SUBPS/SUBPD/SUBSS/SUBSD xmm, xmm/m */
    /* 5D */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* MINPS/MINPD/MINSS/MINSD xmm, xmm/m */
    /* 5E */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* DIVPS/DIVPD/DIVSS/DIVSD xmm, xmm/m */
    /* 5F */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* MAXPS/MAXPD/MAXSS/MAXSD xmm, xmm/m */

    /* 0F 60-6F - MMX instructions */
    /* 60 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PUNPCKLBW mm/xmm, mm/m64/xmm/m128 */
    /* 61 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PUNPCKLWD mm/xmm, mm/m64/xmm/m128 */
    /* 62 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PUNPCKLDQ mm/xmm, mm/m64/xmm/m128 */
    /* 63 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PACKSSWB mm/xmm, mm/m64/xmm/m128 */
    /* 64 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PCMPGTB mm/xmm, mm/m64/xmm/m128 */
    /* 65 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PCMPGTW mm/xmm, mm/m64/xmm/m128 */
    /* 66 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PCMPGTD mm/xmm, mm/m64/xmm/m128 */
    /* 67 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PACKUSWB mm/xmm, mm/m64/xmm/m128 */
    /* 68 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PUNPCKHBW mm/xmm, mm/m64/xmm/m128 */
    /* 69 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PUNPCKHWD mm/xmm, mm/m64/xmm/m128 */
    /* 6A */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PUNPCKHDQ mm/xmm, mm/m64/xmm/m128 */
    /* 6B */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PACKSSDW mm/xmm, mm/m64/xmm/m128 */
    /* 6C */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },              /* PUNPCKLQDQ xmm1, xmm2/m128 */
    /* 6D */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },              /* PUNPCKHQDQ xmm1, xmm2/m128 */
    /* 6E */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVD/MOVQ mm/xmm, r/m32/64 */
    /* 6F */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP, F_NONE }, /* MOVQ/MOVDQA/MOVDQU mm/xmm, mm/m64/xmm/m128 */

    /* 0F 70-7F - SSE instructions (PSHUFW, etc.) */
    /* 70 */ { C_MODRM | C_IMM8, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* PSHUFW/PSHUFD/PSHUFHW/PSHUFLW mm/xmm, mm/xmm/m, imm8 */
    /* 71 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY | PREFIX_OPERAND_SIZE, GROUP_12 }, /* PSRLW/PSRAW/PSLLW xmm, imm8 */
    /* 72 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY | PREFIX_OPERAND_SIZE, GROUP_13 }, /* PSRLD/PSRAD/PSLLD xmm, imm8 */
    /* 73 */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY | PREFIX_OPERAND_SIZE, GROUP_14 }, /* PSRLQ/PSRLDQ/PSLLQ/PSLLDQ xmm, imm8 */
    /* 74 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PCMPEQB mm/xmm, mm/m64/xmm/m128 */
    /* 75 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PCMPEQW mm/xmm, mm/m64/xmm/m128 */
    /* 76 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PCMPEQD mm/xmm, mm/m64/xmm/m128 */
    /* 77 */ { C_NONE, PRE_NONE, F_NONE },             /* EMMS */
    /* 78 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* VMREAD/EXTRQ/INSERTQ r/m, r */
    /* 79 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* VMWRITE/EXTRQ/INSERTQ r, r/m */
    /* 7A */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 7B */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* 7C */ { C_MODRM, PREFIX_OPERAND_SIZE | PREFIX_REPNE, F_NONE },     /* HADDPD/HADDPS xmm1, xmm2/m128 */
    /* 7D */ { C_MODRM, PREFIX_OPERAND_SIZE | PREFIX_REPNE, F_NONE },     /* HSUBPD/HSUBPS xmm1, xmm2/m128 */
    /* 7E */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP, F_NONE }, /* MOVD/MOVQ r/m32/64, mm/xmm / MOVQ xmm, xmm/m64 */
    /* 7F */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP, F_NONE }, /* MOVQ/MOVDQA/MOVDQU mm/m64/xmm/m128, mm/xmm */

    /* 0F 80-8F - Long-displacement JCC (Jcc rel32) */
    /* 80 */ { C_REL32, PRE_ANY, F_NONE },             /* JO rel32 */
    /* 81 */ { C_REL32, PRE_ANY, F_NONE },             /* JNO rel32 */
    /* 82 */ { C_REL32, PRE_ANY, F_NONE },             /* JB/JNAE/JC rel32 */
    /* 83 */ { C_REL32, PRE_ANY, F_NONE },             /* JNB/JAE/JNC rel32 */
    /* 84 */ { C_REL32, PRE_ANY, F_NONE },             /* JZ/JE rel32 */
    /* 85 */ { C_REL32, PRE_ANY, F_NONE },             /* JNZ/JNE rel32 */
    /* 86 */ { C_REL32, PRE_ANY, F_NONE },             /* JBE/JNA rel32 */
    /* 87 */ { C_REL32, PRE_ANY, F_NONE },             /* JNBE/JA rel32 */
    /* 88 */ { C_REL32, PRE_ANY, F_NONE },             /* JS rel32 */
    /* 89 */ { C_REL32, PRE_ANY, F_NONE },             /* JNS rel32 */
    /* 8A */ { C_REL32, PRE_ANY, F_NONE },             /* JP/JPE rel32 */
    /* 8B */ { C_REL32, PRE_ANY, F_NONE },             /* JNP/JPO rel32 */
    /* 8C */ { C_REL32, PRE_ANY, F_NONE },             /* JL/JNGE rel32 */
    /* 8D */ { C_REL32, PRE_ANY, F_NONE },             /* JNL/JGE rel32 */
    /* 8E */ { C_REL32, PRE_ANY, F_NONE },             /* JLE/JNG rel32 */
    /* 8F */ { C_REL32, PRE_ANY, F_NONE },             /* JNLE/JG rel32 */

    /* 0F 90-9F - SETcc instructions */
    /* 90 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETO r/m8 */
    /* 91 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNO r/m8 */
    /* 92 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETB/SETNAE/SETC r/m8 */
    /* 93 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNB/SETAE/SETNC r/m8 */
    /* 94 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETZ/SETE r/m8 */
    /* 95 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNZ/SETNE r/m8 */
    /* 96 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETBE/SETNA r/m8 */
    /* 97 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNBE/SETA r/m8 */
    /* 98 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETS r/m8 */
    /* 99 */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNS r/m8 */
    /* 9A */ { C_MODRM, PRE_ANY, F_NONE },             /* SETP/SETPE r/m8 */
    /* 9B */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNP/SETPO r/m8 */
    /* 9C */ { C_MODRM, PRE_ANY, F_NONE },             /* SETL/SETNGE r/m8 */
    /* 9D */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNL/SETGE r/m8 */
    /* 9E */ { C_MODRM, PRE_ANY, F_NONE },             /* SETLE/SETNG r/m8 */
    /* 9F */ { C_MODRM, PRE_ANY, F_NONE },             /* SETNLE/SETG r/m8 */

    /* 0F A0-AF - PUSH FS, POP FS, CPUID, BT, SHLD, PUSH GS, POP GS, RSM, BTS, SHRD, IMUL */
    /* A0 */ { C_NONE, PRE_NONE, F_NONE },             /* PUSH FS */
    /* A1 */ { C_NONE, PRE_NONE, F_NONE },             /* POP FS */
    /* A2 */ { C_NONE, PRE_NONE, F_NONE },             /* CPUID */
    /* A3 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* BT r/m16/32/64, r16/32/64 */
    /* A4 */ { C_MODRM | C_IMM8, PRE_ANY | PRE_LOCK, F_NONE }, /* SHLD r/m16/32/64, r16/32/64, imm8 */
    /* A5 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* SHLD r/m16/32/64, r16/32/64, CL */
    /* A6 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* A7 */ { C_ERROR, PRE_NONE, F_NONE },            /* INVALID */
    /* A8 */ { C_NONE, PRE_NONE, F_NONE },             /* PUSH GS */
    /* A9 */ { C_NONE, PRE_NONE, F_NONE },             /* POP GS */
    /* AA */ { C_NONE, PRE_NONE, F_NONE },             /* RSM */
    /* AB */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* BTS r/m16/32/64, r16/32/64 */
    /* AC */ { C_MODRM | C_IMM8, PRE_ANY | PRE_LOCK, F_NONE }, /* SHRD r/m16/32/64, r16/32/64, imm8 */
    /* AD */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* SHRD r/m16/32/64, r16/32/64, CL */
    /* AE */ { C_MODRM | C_GROUP, PRE_ANY, GROUP_15 }, /* FXSAVE/FXRSTOR/LDMXCSR/STMXCSR/XSAVE/XRSTOR/CLFLUSH */
    /* AF */ { C_MODRM, PRE_ANY, F_NONE },             /* IMUL r16/32/64, r/m16/32/64 */

    /* 0F B0-BF - CMPXCHG, LSS, BTR, LFS, LGS, MOVZX, POPCNT, BTR, BSF, BSR, MOVSX */
    /* B0 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* CMPXCHG r/m8, r8 */
    /* B1 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* CMPXCHG r/m16/32/64, r16/32/64 */
    /* B2 */ { C_MODRM, PRE_ANY, F_NONE },             /* LSS r16/32/64, m16:16/32 */
    /* B3 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* BTR r/m16/32/64, r16/32/64 */
    /* B4 */ { C_MODRM, PRE_ANY, F_NONE },             /* LFS r16/32/64, m16:16/32 */
    /* B5 */ { C_MODRM, PRE_ANY, F_NONE },             /* LGS r16/32/64, m16:16/32 */
    /* B6 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVZX r16/32/64, r/m8 */
    /* B7 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVZX r16/32/64, r/m16 */
    /* B8 */ { C_MODRM, PREFIX_REP, F_NONE },          /* POPCNT r16/32/64, r/m16/32/64 (with F3 prefix) */
    /* B9 */{ C_MODRM, PRE_ANY, F_NONE },             /* UD1 r16/32/64, r/m16/32/64 */
    /* BA */ { C_MODRM | C_IMM8 | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_8 }, /* BT/BTS/BTR/BTC r/m16/32/64, imm8 */
    /* BB */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* BTC r/m16/32/64, r16/32/64 */
    /* BC */ { C_MODRM, PRE_ANY | PREFIX_REP, F_NONE },    /* BSF/TZCNT r16/32/64, r/m16/32/64 */
    /* BD */ { C_MODRM, PRE_ANY | PREFIX_REP, F_NONE },    /* BSR/LZCNT r16/32/64, r/m16/32/64 */
    /* BE */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVSX r16/32/64, r/m8 */
    /* BF */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVSX r16/32/64, r/m16 */

    /* 0F C0-CF - XADD, SSE, MOVNTI, PINSRW, PEXTRW, SHUFPS, CMPXCHG8B, VMPTRLD, VMXON, BSWAP */
    /* C0 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* XADD r/m8, r8 */
    /* C1 */ { C_MODRM, PRE_ANY | PRE_LOCK, F_NONE },  /* XADD r/m16/32/64, r16/32/64 */
    /* C2 */ { C_MODRM | C_IMM8, PRE_ANY | PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CMPPS/CMPPD/CMPSS/CMPSD xmm1, xmm2/m, imm8 */
    /* C3 */ { C_MODRM, PRE_ANY, F_NONE },             /* MOVNTI m32/64, r32/64 */
    /* C4 */ { C_MODRM | C_IMM8, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE }, /* PINSRW mm/xmm, r32/m16, imm8 */
    /* C5 */ { C_MODRM | C_IMM8, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE }, /* PEXTRW r32, mm/xmm, imm8 */
    /* C6 */ { C_MODRM | C_IMM8, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE }, /* SHUFPS/SHUFPD xmm1, xmm2/m128, imm8 */
    /* C7 */ { C_MODRM | C_GROUP, PRE_ANY | PRE_LOCK, GROUP_9 }, /* CMPXCHG8B/CMPXCHG16B/VMPTRLD/VMCLEAR/VMXON */
    /* C8 */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RAX/EAX */
    /* C9 */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RCX/ECX */
    /* CA */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RDX/EDX */
    /* CB */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RBX/EBX */
    /* CC */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RSP/ESP */
    /* CD */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RBP/EBP */
    /* CE */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RSI/ESI */
    /* CF */ { C_NONE, PRE_NONE, F_NONE },             /* BSWAP RDI/EDI */

    /* 0F D0-DF - SSE instructions (continued) */
    /* D0 */ { C_MODRM, PREFIX_OPERAND_SIZE | PREFIX_REPNE, F_NONE },     /* ADDSUBPD/ADDSUBPS xmm1, xmm2/m128 */
    /* D1 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSRLW mm/xmm, mm/m64/xmm/m128 */
    /* D2 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSRLD mm/xmm, mm/m64/xmm/m128 */
    /* D3 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSRLQ mm/xmm, mm/m64/xmm/m128 */
    /* D4 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDQ mm/xmm, mm/m64/xmm/m128 */
    /* D5 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMULLW mm/xmm, mm/m64/xmm/m128 */
    /* D6 */ { C_MODRM, PREFIX_OPERAND_SIZE | PREFIX_REP, F_NONE },     /* MOVQ xmm/m64, xmm / MOVQ2DQ xmm, mm */
    /* D7 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMOVMSKB r32, mm/xmm */
    /* D8 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBUSB mm/xmm, mm/m64/xmm/m128 */
    /* D9 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBUSW mm/xmm, mm/m64/xmm/m128 */
    /* DA */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMINUB mm/xmm, mm/m64/xmm/m128 */
    /* DB */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PAND mm/xmm, mm/m64/xmm/m128 */
    /* DC */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDUSB mm/xmm, mm/m64/xmm/m128 */
    /* DD */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDUSW mm/xmm, mm/m64/xmm/m128 */
    /* DE */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMAXUB mm/xmm, mm/m64/xmm/m128 */
    /* DF */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PANDN mm/xmm, mm/m64/xmm/m128 */

    /* 0F E0-EF - SSE instructions (continued) */
    /* E0 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PAVGB mm/xmm, mm/m64/xmm/m128 */
    /* E1 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSRAW mm/xmm, mm/m64/xmm/m128 */
    /* E2 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSRAD mm/xmm, mm/m64/xmm/m128 */
    /* E3 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PAVGW mm/xmm, mm/m64/xmm/m128 */
    /* E4 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMULHUW mm/xmm, mm/m64/xmm/m128 */
    /* E5 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMULHW mm/xmm, mm/m64/xmm/m128 */
    /* E6 */ { C_MODRM, PREFIX_OPERAND_SIZE | PREFIX_REP | PREFIX_REPNE, F_NONE }, /* CVTTPD2DQ/CVTDQ2PD/CVTPD2DQ xmm1, xmm2/m128 */
    /* E7 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MOVNTQ/MOVNTDQ m64/m128, mm/xmm */
    /* E8 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBSB mm/xmm, mm/m64/xmm/m128 */
    /* E9 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBSW mm/xmm, mm/m64/xmm/m128 */
    /* EA */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMINSW mm/xmm, mm/m64/xmm/m128 */
    /* EB */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* POR mm/xmm, mm/m64/xmm/m128 */
    /* EC */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDSB mm/xmm, mm/m64/xmm/m128 */
    /* ED */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDSW mm/xmm, mm/m64/xmm/m128 */
    /* EE */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMAXSW mm/xmm, mm/m64/xmm/m128 */
    /* EF */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PXOR mm/xmm, mm/m64/xmm/m128 */

    /* 0F F0-FF - SSE instructions (final block) */
    /* F0 */ { C_MODRM, PREFIX_REPNE, F_NONE },              /* LDDQU xmm, m128 */
    /* F1 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSLLW mm/xmm, mm/m64/xmm/m128 */
    /* F2 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSLLD mm/xmm, mm/m64/xmm/m128 */
    /* F3 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSLLQ mm/xmm, mm/m64/xmm/m128 */
    /* F4 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMULUDQ mm/xmm, mm/m64/xmm/m128 */
    /* F5 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PMADDWD mm/xmm, mm/m64/xmm/m128 */
    /* F6 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSADBW mm/xmm, mm/m64/xmm/m128 */
    /* F7 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* MASKMOVQ/MASKMOVDQU mm/xmm, mm/xmm */
    /* F8 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBB mm/xmm, mm/m64/xmm/m128 */
    /* F9 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBW mm/xmm, mm/m64/xmm/m128 */
    /* FA */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBD mm/xmm, mm/m64/xmm/m128 */
    /* FB */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PSUBQ mm/xmm, mm/m64/xmm/m128 */
    /* FC */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDB mm/xmm, mm/m64/xmm/m128 */
    /* FD */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDW mm/xmm, mm/m64/xmm/m128 */
    /* FE */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },    /* PADDD mm/xmm, mm/m64/xmm/m128 */
    /* FF */ { C_ERROR, PRE_NONE, F_NONE }             /* INVALID */
};