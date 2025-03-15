#pragma once


#include <stdint.h>

/* Maximum instruction length */
#define MAX_INSTRUCTION_LENGTH 15

/* Maximum prefixes */
#define MAX_LEGACY_PREFIXES 4

/* 2byte Opcode prefix */
#define TWO_BYTE_OPCODE 0x0F
/* 3byte Opcode prefix */
#define THREE_BYTE_OPCODE_38 0x38
#define THREE_BYTE_OPCODE_3A 0x3A

/* ----------------------------------------------- Instruction flags ----------------------------------------------- */
 /* Component presence flags */
#define F_MODRM         0x00000001  /* Has ModR/M byte */
#define F_SIB           0x00000002  /* Has SIB byte */
#define F_IMM8          0x00000004  /* Has 8-bit immediate */
#define F_IMM16         0x00000008  /* Has 16-bit immediate */
#define F_IMM32         0x00000010  /* Has 32-bit immediate */
#define F_IMM64         0x00000020  /* Has 64-bit immediate */
#define F_DISP8         0x00000040  /* Has 8-bit displacement */
#define F_DISP16        0x00000080  /* Has 16-bit displacement */
#define F_DISP32        0x00000100  /* Has 32-bit displacement */
#define F_RELATIVE      0x00000200  /* Has relative address (jump/call) */
#define F_IP_RELATIVE   0x00000400  /* Uses RIP-relative addressing */

/* Error flags */
#define F_ERROR         0x00001000  /* General error */
#define F_ERROR_OPCODE  0x00002000  /* Invalid opcode */
#define F_ERROR_LENGTH  0x00004000  /* Instruction too long */
#define F_ERROR_LOCK    0x00008000  /* Invalid lock prefix */
#define F_ERROR_OPERAND 0x00010000  /* Invalid operand */

/* Prefix flags for instruction structure */
#define F_PREFIX_REPNZ  0x01000000  /* REPNZ/REPNE prefix (F2) */
#define F_PREFIX_REPZ   0x02000000  /* REPZ/REPE prefix (F3) */
#define F_PREFIX_REP    0x03000000  /* REP prefix (either REPNZ or REPZ) */
#define F_PREFIX_66     0x04000000  /* Operand size override (66) */
#define F_PREFIX_67     0x08000000  /* Address size override (67) */
#define F_PREFIX_LOCK   0x10000000  /* LOCK prefix (F0) */
#define F_PREFIX_SEG    0x20000000  /* Segment override prefix */
#define F_PREFIX_REX    0x40000000  /* REX prefix (4x) */
#define F_PREFIX_ANY    0x7F000000  /* Any prefix */


/* ----------------------------------------------- Instruction flags ----------------------------------------------- */



/* ----------------------------------------------- Prefix byte ----------------------------------------------- */ 
#define PREFIX_NONE         0x00  /* No prefix */
#define PREFIX_LOCK         0xF0  /* LOCK - Assert lock signal */
#define PREFIX_REPNE        0xF2  /* REPNE/REPNZ - Repeat not equal/zero */
#define PREFIX_REP          0xF3  /* REP/REPE/REPZ - Repeat equal/zero */
#define PREFIX_CS           0x2E  /* CS segment override */
#define PREFIX_SS           0x36  /* SS segment override */
#define PREFIX_DS           0x3E  /* DS segment override */
#define PREFIX_ES           0x26  /* ES segment override */
#define PREFIX_FS           0x64  /* FS segment override */
#define PREFIX_GS           0x65  /* GS segment override */
#define PREFIX_OPERAND_SIZE 0x66  /* Operand size override */
#define PREFIX_ADDRESS_SIZE 0x67  /* Address size override */
 /* ----------------------------------------------- Prefix byte ----------------------------------------------- */





/* ----------------------------------------------- Valid prefixes for instruction table ----------------------------------------------- */
#define PRE_ANY                 0x00  /* No specific prefix required */
#define PRE_NONE                0x01  /* No prefix allowed */
#define PRE_REPN                0x02  /* REPNE/REPNZ prefix */
#define PRE_REP                 0x04  /* REP/REPE/REPZ prefix */
#define PRE_OPERAND_OVERRIDE    0x08  /* Operand size override prefix */
#define PRE_ADDRESS_OVERRIDE    0x10  /* Address size override prefix */
#define PRE_LOCK                0x20  /* LOCK prefix */
#define PRE_SEG                 0x40  /* Segment override prefix */
#define PRE_ALL                 0xFF  /* All prefixes allowed */
/* ----------------------------------------------- Valid prefixes for instruction table ----------------------------------------------- */


/* ------------------------ Instruction Characteristics Used in opcode tables to describe instruction properties ------------------------ */
#define C_NONE    0x00  /* No special characteristics */
#define C_MODRM   0x01  /* Has ModR/M byte */
#define C_IMM8    0x02  /* Has 8-bit immediate */
#define C_IMM16   0x04  /* Has 16-bit immediate */
#define C_IMM_P66 0x10  /* Operand size depends on 66 prefix (16/32-bit imm) */
#define C_REL8    0x20  /* Has 8-bit relative offset */
#define C_REL32   0x40  /* Has 32-bit relative offset */
#define C_GROUP   0x80  /* Is a group opcode, reg field selects instruction */
#define C_ERROR   0xFF  /* Invalid opcode */
/* ------------------------ Instruction Characteristics Used in opcode tables to describe instruction properties ------------------------ */



/*
* Additional instruction flags
*/
#define F_NONE     0x00  /* No additional flags */
#define F_SPECIAL  0x01  /* Special case handling required */
#define F_64MODE   0x02  /* 64-bit mode only instruction */
#define F_DEFAULT  0x04  /* Default operand size is 64-bit in 64-bit mode */
#define F_REX_W    0x08  /* REX.W affects operand size */

/*
* Group numbers
* Used to identify opcode groups
*/
#define GROUP_NONE 0   /* Place holder */
#define GROUP_1   1   /* Group 1: ADD, OR, ADC, SBB, AND, SUB, XOR, CMP */
#define GROUP_2   2   /* Group 2: ROL, ROR, RCL, RCR, SHL, SHR, SAL, SAR */
#define GROUP_3   3   /* Group 3: TEST, NOT, NEG, MUL, IMUL, DIV, IDIV */
#define GROUP_4   4   /* Group 4: INC, DEC */
#define GROUP_5   5   /* Group 5: INC, DEC, CALL, CALL, JMP, JMP, PUSH */
#define GROUP_6   6   /* Group 6: SLDT, STR, LLDT, LTR, VERR, VERW */
#define GROUP_7   7   /* Group 7: SGDT, SIDT, LGDT, LIDT, SMSW, LMSW, INVLPG */
#define GROUP_8   8   /* Group 8: BT, BTS, BTR, BTC */
#define GROUP_9   9   /* Group 9: (various) */
#define GROUP_10  10  /* Group 10: (various) */
#define GROUP_11  11  /* Group 11: MOV (immediate to reg/mem) */
#define GROUP_12  12  /* Group 12: (various, mainly SSE) */
#define GROUP_13  13  /* Group 13: (various, mainly SSE) */
#define GROUP_14  14  /* Group 14: (various, mainly SSE) */
#define GROUP_15  15  /* Group 15: (various, mainly SSE) */
#define GROUP_16  16  /* Group 16: (various, mainly SSE) */
#define GROUP_17  17  /* Group 17: (various, mainly SSE) */
#define GROUP_P   18  /* Group P: (various, prefixed) */

/*
* Opcode table entry structure
* Compact representation of instruction characteristics
*/
typedef struct {
    uint8_t flags;    /* Instruction characteristics (C_xxx) */
    uint8_t prefix;   /* Valid prefix combinations (PRE_xxx) */
    uint8_t group;    /* Additional flags (F_xxx) or group index for C_GROUP */
} opcode_table_entry_t;

/*
 * Group opcode entry
 * For group opcodes, this defines the characteristics for each value of ModR/M.reg (0-7)
 */
typedef struct {
    uint8_t modrm;    /* ModR/M value (used only for identification) */
    uint8_t index;    /* Group index (GROUP_x) */
    opcode_table_entry_t entries[8]; /* One entry for each value of ModR/M.reg */
} group_table_entry_t;