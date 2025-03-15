#include <stdint.h>
#include <stdbool.h>  /* For bool type */
#include "x86_opcode_defs.h"  /* For all the constants and definitions */

#define IS_REX_BYTE(b) (b >= 0x40 && b <= 0x4F)
#define IS_MOV_R64_IMM64_RANGE(b) (b >= 0xB8 && b <= 0xBF)
#define GET_REX_W(b) ((b >> 3) & 1)
#define GET_REX_R(b) ((b >> 2) & 1)
#define GET_REX_X(b) ((b >> 1) & 1)
#define GET_REX_B(b) (b & 1)
#define GET_MODRM_MOD(b) (b >> 6)
#define GET_MODRM_REG(b) ((b >> 3) & 7)
#define GET_MODRM_RM(b) (b & 7)
#define GET_SIB_SCALE(b) ((b >> 6) & 3)
#define GET_SIB_INDEX(b) ((b >> 3) & 7)  
#define GET_SIB_BASE(b) (b & 7)         


    /* Define the main instruction structure */
    typedef struct {
        uint8_t length;                     /* Length of instruction in bytes */
        uint8_t bytes[MAX_INSTRUCTION_LENGTH]; /* Raw instruction bytes */

        /* Prefix information */
        uint8_t prefix_count;               /* Number of prefixes */
        uint8_t prefixes[4];                /* Array of up to 4 prefixes */
        uint8_t p_rep;                      /* REP/REPNE prefix (F3/F2) */
        uint8_t p_repne;                    /* REP/REPNE prefix (F3/F2) */
        uint8_t p_lock;                     /* LOCK prefix (F0) */
        uint8_t p_seg;                      /* Segment override */
        uint8_t p_66;                       /* Operand size override (66) */
        uint8_t p_67;                       /* Address size override (67) */

        /* REX prefix */
        uint8_t rex;                        /* REX prefix byte */
        uint8_t rex_w;                      /* REX.W bit (64-bit operand) */
        uint8_t rex_r;                      /* REX.R bit (ModR/M reg extension) */
        uint8_t rex_x;                      /* REX.X bit (SIB index extension) */
        uint8_t rex_b;                      /* REX.B bit (ModR/M r/m extension) */

        /* Opcode */
        uint8_t opcode;                     /* Primary opcode byte */
        uint8_t opcode2;                    /* Secondary opcode byte (for 2/3-byte opcodes) */
        uint8_t opcode3;                    /* Tertiary opcode byte (for 3-byte opcodes) */

        /* ModR/M */
        uint8_t modrm;                      /* ModR/M byte */
        uint8_t modrm_mod;                  /* ModR/M mod field (2 bits) */
        uint8_t modrm_reg;                  /* ModR/M reg field (3 bits) */
        uint8_t modrm_rm;                   /* ModR/M r/m field (3 bits) */

        /* SIB */
        uint8_t sib;                        /* SIB byte */
        uint8_t sib_scale;                  /* SIB scale field (2 bits) */
        uint8_t sib_index;                  /* SIB index field (3 bits) */
        uint8_t sib_base;                   /* SIB base field (3 bits) */

        /* Displacement */
        union {
            uint8_t disp8;                  /* 8-bit displacement */
            uint16_t disp16;                /* 16-bit displacement */
            uint32_t disp32;                /* 32-bit displacement */
        } disp;
        uint8_t disp_size;                  /* Size of displacement (0, 1, 2, 4) */

        /* Immediate value */
        union {
            uint8_t imm8;                   /* 8-bit immediate */
            uint16_t imm16;                 /* 16-bit immediate */
            uint32_t imm32;                 /* 32-bit immediate */
            uint64_t imm64;                 /* 64-bit immediate */
        } imm;
        uint8_t imm_size;                   /* Size of immediate (0, 1, 2, 4, 8) */

        /* Flags */
        uint32_t flags;                     /* Instruction flags */
    } x86_instruction_t;

    /* Function declarations */
  
int x86_disassembler(const uint8_t* code, x86_instruction_t* inst);
const char* register_name(int reg, int size);
void print_bytes(const uint8_t* bytes, int length);
void print_flags(uint32_t flags);
void print_disassembly(const x86_instruction_t* inst, uint64_t address);
void format_instruction(const x86_instruction_t* inst, char* buffer, size_t size);
