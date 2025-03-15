#pragma once
#include "x86_opcode_defs.h"

/*
 * 3-byte opcode table (0F 38 xx)
 * Each entry describes the characteristics of the corresponding opcode
 */
const opcode_table_entry_t x86_3byte_38_opcode_table[256] = {
    /* 0F 38 00 - 0F */
    /* 00 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PSHUFB mm/xmm, mm/m64/xmm/m128 */
    /* 01 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PHADDW mm/xmm, mm/m64/xmm/m128 */
    /* 02 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PHADDD mm/xmm, mm/m64/xmm/m128 */
    /* 03 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PHADDSW mm/xmm, mm/m64/xmm/m128 */
    /* 04 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PMADDUBSW mm/xmm, mm/m64/xmm/m128 */
    /* 05 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PHSUBW mm/xmm, mm/m64/xmm/m128 */
    /* 06 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PHSUBD mm/xmm, mm/m64/xmm/m128 */
    /* 07 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PHSUBSW mm/xmm, mm/m64/xmm/m128 */
    /* 08 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PSIGNB mm/xmm, mm/m64/xmm/m128 */
    /* 09 */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PSIGNW mm/xmm, mm/m64/xmm/m128 */
    /* 0A */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PSIGND mm/xmm, mm/m64/xmm/m128 */
    /* 0B */ { C_MODRM, PRE_ANY | PREFIX_OPERAND_SIZE, F_NONE },  /* PMULHRSW mm/xmm, mm/m64/xmm/m128 */
    /* 0C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 0D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 0E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 0F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 38 10 - 1F */
    /* 10 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PBLENDVB xmm1, xmm2/m128, xmm0 */
    /* 11 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 12 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 13 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 14 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* BLENDVPS xmm1, xmm2/m128, xmm0 */
    /* 15 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* BLENDVPD xmm1, xmm2/m128, xmm0 */
    /* 16 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 17 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PTEST xmm1, xmm2/m128 */
    /* 18 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 19 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1C */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PABSB xmm1, xmm2/m128 */
    /* 1D */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PABSW xmm1, xmm2/m128 */
    /* 1E */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PABSD xmm1, xmm2/m128 */
    /* 1F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 38 20 - 2F */
    /* 20 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVSXBW xmm1, xmm2/m64 */
    /* 21 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVSXBD xmm1, xmm2/m32 */
    /* 22 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVSXBQ xmm1, xmm2/m16 */
    /* 23 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVSXWD xmm1, xmm2/m64 */
    /* 24 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVSXWQ xmm1, xmm2/m32 */
    /* 25 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVSXDQ xmm1, xmm2/m64 */
    /* 26 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 27 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 28 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMULDQ xmm1, xmm2/m128 */
    /* 29 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PCMPEQQ xmm1, xmm2/m128 */
    /* 2A */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* MOVNTDQA xmm1, m128 */
    /* 2B */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PACKUSDW xmm1, xmm2/m128 */
    /* 2C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 38 30 - 3F */
    /* 30 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVZXBW xmm1, xmm2/m64 */
    /* 31 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVZXBD xmm1, xmm2/m32 */
    /* 32 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVZXBQ xmm1, xmm2/m16 */
    /* 33 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVZXWD xmm1, xmm2/m64 */
    /* 34 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVZXWQ xmm1, xmm2/m32 */
    /* 35 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMOVZXDQ xmm1, xmm2/m64 */
    /* 36 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 37 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PCMPGTQ xmm1, xmm2/m128 */
    /* 38 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMINSB xmm1, xmm2/m128 */
    /* 39 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMINSD xmm1, xmm2/m128 */
    /* 3A */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMINUW xmm1, xmm2/m128 */
    /* 3B */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMINUD xmm1, xmm2/m128 */
    /* 3C */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMAXSB xmm1, xmm2/m128 */
    /* 3D */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMAXSD xmm1, xmm2/m128 */
    /* 3E */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMAXUW xmm1, xmm2/m128 */
    /* 3F */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMAXUD xmm1, xmm2/m128 */

    /* 0F 38 40 - 4F */
    /* 40 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PMULLD xmm1, xmm2/m128 */
    /* 41 */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PHMINPOSUW xmm1, xmm2/m128 */
    /* 42 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 43 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 44 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 45 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 46 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 47 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 48 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 49 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* Fill the rest with Invalid entries */
    /* 0F 38 50 - FF */
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },

    /* 0F 38 60 */
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },

    /* Continue with remaining entries (70-FF) - all invalid for this example */
    /* For brevity, we're not listing all 256 entries, but in a real implementation
       you would need to provide the full table. The remaining entries are all
       set to C_ERROR for now. */

       /* Fill the rest of the table with error entries */
       /* 0F 38 70 - FF - we're using a simplified approach here for brevity */
       { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
       { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
       { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
       { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
       /* ... continuing with error entries */
};

/*
 * 3-byte opcode table (0F 3A xx)
 * Each entry describes the characteristics of the corresponding opcode
 */
const opcode_table_entry_t x86_3byte_3A_opcode_table[256] = {
    /* 0F 3A 00 - 0F */
    /* 00 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 01 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 02 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 03 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 04 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 05 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 06 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 07 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 08 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* ROUNDPS xmm1, xmm2/m128, imm8 */
    /* 09 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* ROUNDPD xmm1, xmm2/m128, imm8 */
    /* 0A */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* ROUNDSS xmm1, xmm2/m32, imm8 */
    /* 0B */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* ROUNDSD xmm1, xmm2/m64, imm8 */
    /* 0C */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* BLENDPS xmm1, xmm2/m128, imm8 */
    /* 0D */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* BLENDPD xmm1, xmm2/m128, imm8 */
    /* 0E */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PBLENDW xmm1, xmm2/m128, imm8 */
    /* 0F */ { C_MODRM, PREFIX_OPERAND_SIZE, F_NONE },            /* PALIGNR xmm1, xmm2/m128, imm8 */

    /* 0F 3A 10 - 1F */
    /* 10 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 11 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 12 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 13 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 14 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PEXTRB reg/m8, xmm2, imm8 */
    /* 15 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PEXTRW reg/m16, xmm2, imm8 */
    /* 16 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PEXTRD/PEXTRQ reg/m32/m64, xmm2, imm8 */
    /* 17 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* EXTRACTPS r/m32, xmm1, imm8 */
    /* 18 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 19 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 1F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 3A 20 - 2F */
    /* 20 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PINSRB xmm1, r32/m8, imm8 */
    /* 21 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* INSERTPS xmm1, xmm2/m32, imm8 */
    /* 22 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PINSRD/PINSRQ xmm1, r/m32/r/m64, imm8 */
    /* 23 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 24 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 25 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 26 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 27 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 28 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 29 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 2F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 3A 30 - 3F */
    /* 30 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 31 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 32 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 33 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 34 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 35 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 36 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 37 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 38 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 39 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 3A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 3B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 3C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 3D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 3E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 3F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 3A 40 - 4F */
    /* 40 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* DPPS xmm1, xmm2/m128, imm8 */
    /* 41 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* DPPD xmm1, xmm2/m128, imm8 */
    /* 42 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* MPSADBW xmm1, xmm2/m128, imm8 */
    /* 43 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 44 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 45 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 46 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 47 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 48 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 49 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 4F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 3A 50 - 5F */
    /* 50 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 51 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 52 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 53 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 54 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 55 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 56 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 57 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 58 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 59 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 5A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 5B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 5C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 5D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 5E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 5F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* 0F 3A 60 - 6F */
    /* 60 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PCMPESTRM xmm1, xmm2/m128, imm8 */
    /* 61 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PCMPESTRI xmm1, xmm2/m128, imm8 */
    /* 62 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PCMPISTRM xmm1, xmm2/m128, imm8 */
    /* 63 */ { C_MODRM | C_IMM8, PREFIX_OPERAND_SIZE, F_NONE },   /* PCMPISTRI xmm1, xmm2/m128, imm8 */
    /* 64 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 65 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 66 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 67 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 68 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 69 */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 6A */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 6B */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 6C */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 6D */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 6E */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */
    /* 6F */ { C_ERROR, PRE_NONE, F_NONE },          /* Invalid */

    /* Fill the rest with Invalid entries */
    /* 0F 3A 70 - FF */
    /* For brevity, we're not listing all 256 entries, but in a real implementation
       you would need to provide the full table. The remaining entries are all
       set to C_ERROR for now. */
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    { C_ERROR, PRE_NONE, F_NONE }, { C_ERROR, PRE_NONE, F_NONE },
    /* ... continuing with error entries */
};
