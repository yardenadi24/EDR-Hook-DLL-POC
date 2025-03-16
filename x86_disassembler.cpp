/*
 * x86-64 Disassembler
 * Improved implementation based on the HDE64 approach
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pch.h"
#include "x86_instruction.h"

 /* Include the opcode tables */
#include "x86_1byte_table.h"
#include "x86_2byte_table.h"
#include "x86_3byte_tables.h"
#include "x86_group_tables.h"


/*
 * Parse legacy prefixes
 * Returns the number of bytes consumed
 */
static int parse_prefixes(
    const uint8_t* code,
    int offset,
    x86_instruction_t* inst)
{
    uint8_t* p = (uint8_t*)(code + offset);
    int prefix_count = 0;
    uint8_t pref = 0;

    for (int i = 0; i < MAX_LEGACY_PREFIXES; i++) {
        uint8_t c = *p;

        switch (c) {
        case PREFIX_REP:
            inst->p_rep = c;
            pref |= PRE_REP;
            inst->flags |= F_PREFIX_REP;
            inst->prefixes[inst->prefix_count] = c;
            inst->prefix_count++;
            p++;
            prefix_count++;
            break;

        case PREFIX_REPNE:
            inst->p_repne = c;
            pref |= PRE_REPN;
            inst->flags |= F_PREFIX_REPNZ;
            inst->prefixes[inst->prefix_count] = c;
            inst->prefix_count++;
            p++;
            prefix_count++;
            break;

        case PREFIX_LOCK:
            inst->p_lock = c;
            pref |= PRE_LOCK;
            inst->flags |= F_PREFIX_LOCK;
            inst->prefixes[inst->prefix_count] = c;
            inst->prefix_count++;
            p++;
            prefix_count++;
            break;

        case PREFIX_CS:
        case PREFIX_SS:
        case PREFIX_DS:
        case PREFIX_ES:
        case PREFIX_FS:
        case PREFIX_GS:
            inst->p_seg = c;
            pref |= PRE_SEG;
            inst->flags |= F_PREFIX_SEG;
            inst->prefixes[inst->prefix_count] = c;
            inst->prefix_count++;
            p++;
            prefix_count++;
            break;

        case PREFIX_OPERAND_SIZE:
            inst->p_66 = 1;
            pref |= PRE_OPERAND_OVERRIDE;
            inst->flags |= F_PREFIX_66;
            inst->prefixes[inst->prefix_count] = c;
            inst->prefix_count++;
            p++;
            prefix_count++;
            break;

        case PREFIX_ADDRESS_SIZE:
            inst->p_67 = 1;
            pref |= PRE_ADDRESS_OVERRIDE;
            inst->flags |= F_PREFIX_67;
            inst->prefixes[inst->prefix_count] = c;
            inst->prefix_count++;
            p++;
            prefix_count++;
            break;

        default:
            /* Not a prefix, exit the loop */
            return prefix_count;
        }
    }

    return prefix_count;
}

/*
 * Parse REX prefix
 * Returns the number of bytes consumed
 */
static int parse_rex(
    const uint8_t* code,
    int offset,
    x86_instruction_t* inst) 
{
    uint8_t c = code[offset];

    if (IS_REX_BYTE(c)) {
        inst->flags |= F_PREFIX_REX;
        inst->rex = c;

        /* Parse the REX byte */
        inst->rex_w = GET_REX_W(c);
        inst->rex_r = GET_REX_R(c);
        inst->rex_x = GET_REX_X(c);
        inst->rex_b = GET_REX_B(c);

        return 1;
    }

    return 0;
}

/*
 * Handle special case opcodes (like direct memory MOV A0-A3)
 */
static void handle_special_opcodes(
    uint8_t opcode,
    x86_instruction_t* inst,
    opcode_info_t* opcode_info) 
{
    /* Handle special MOV instructions A0-A3 */
    if (opcode >= 0xA0 && opcode <= 0xA3) {
        /* Special case for direct memory access - determine immediate size */
        if (inst->p_67) {
            /* 32-bit address mode */
            opcode_info->imm_type = IMM_TYPE_32;
        }
        else {
            /* 64-bit address mode */
            opcode_info->imm_type = IMM_TYPE_64;
        }

        /* Address size prefix affects the operand itself for these instructions */
        if (inst->flags & F_PREFIX_67) {
            inst->flags |= F_PREFIX_66;
        }
    }
}

/*
 * Parse opcode and determine instruction characteristics
 * Returns the number of bytes consumed
 */
static int parse_opcode(
    const uint8_t* code,
    int offset,
    x86_instruction_t* inst,
    opcode_info_t* opcode_info)
{
    uint8_t* p = (uint8_t*)(code + offset);
    int bytes_consumed = 0;

    /* Clear opcode info */
    memset(opcode_info, 0, sizeof(opcode_info_t));

    /* Get primary opcode */
    uint8_t opcode = *p;
    p++;
    bytes_consumed++;
    inst->opcode = opcode;

    /* Check for MOV r64, imm64 - Special case instruction */
    int op64 = 0;
    if (inst->rex_w && opcode >= 0xB8 && opcode <= 0xBF) {
        op64 = 1;
    }

    /* Check for 2/3 byte opcode */
    if (opcode == TWO_BYTE_OPCODE) {
        /* 2-byte opcode */
        uint8_t opcode2 = *p;
        p++;
        bytes_consumed++;
        inst->opcode2 = opcode2;

        /* Check for 3-byte opcode */
        if (opcode2 == THREE_BYTE_OPCODE_38 || opcode2 == THREE_BYTE_OPCODE_3A) {
            uint8_t opcode3 = *p;
            p++;
            bytes_consumed++;
            inst->opcode3 = opcode3;

            /* Get opcode characteristics from 3-byte table */
            if (opcode2 == THREE_BYTE_OPCODE_38) {
                opcode_info->flags = x86_3byte_38_opcode_table[opcode3].flags;
                opcode_info->valid_prefixes = x86_3byte_38_opcode_table[opcode3].prefix;
                opcode_info->group_index = x86_3byte_38_opcode_table[opcode3].group;
            }
            else {
                opcode_info->flags = x86_3byte_3A_opcode_table[opcode3].flags;
                opcode_info->valid_prefixes = x86_3byte_3A_opcode_table[opcode3].prefix;
                opcode_info->group_index = x86_3byte_3A_opcode_table[opcode3].group;
            }
        }
        else {
            /* Get opcode characteristics from 2-byte table */
            opcode_info->flags = x86_2byte_opcode_table[opcode2].flags;
            opcode_info->valid_prefixes = x86_2byte_opcode_table[opcode2].prefix;
            opcode_info->group_index = x86_2byte_opcode_table[opcode2].group;
        }
    }
    else {
        /* Get opcode characteristics from 1-byte table */
        opcode_info->flags = x86_1byte_opcode_table[opcode].flags;
        opcode_info->valid_prefixes = x86_1byte_opcode_table[opcode].prefix;
        opcode_info->group_index = x86_1byte_opcode_table[opcode].group;

        /* Handle special cases */
        handle_special_opcodes(opcode, inst, opcode_info);
    }

    /* Determine immediate type based on opcode flags */
    if (opcode_info->flags & C_IMM_P66) {
        if (opcode_info->flags & C_REL32) {
            opcode_info->is_relative = 1;
            if (inst->flags & F_PREFIX_66) {
                opcode_info->imm_type = IMM_TYPE_16;
            }
            else {
                opcode_info->imm_type = IMM_TYPE_32;
            }
        }
        else if (op64) {
            opcode_info->imm_type = IMM_TYPE_64;
        }
        else if (inst->flags & F_PREFIX_66) {
            opcode_info->imm_type = IMM_TYPE_16;
        }
        else {
            opcode_info->imm_type = IMM_TYPE_32;
        }
    }
    else if (opcode_info->flags & C_IMM16) {
        opcode_info->imm_type = IMM_TYPE_16;
    }
    else if (opcode_info->flags & C_IMM8) {
        opcode_info->imm_type = IMM_TYPE_8;
    }
    else if (opcode_info->flags & C_REL32) {
        opcode_info->imm_type = IMM_TYPE_REL32;
        opcode_info->is_relative = 1;
    }
    else if (opcode_info->flags & C_REL8) {
        opcode_info->imm_type = IMM_TYPE_REL8;
        opcode_info->is_relative = 1;
    }

    return bytes_consumed;
}

/*
 * Parse ModR/M byte and SIB byte if present
 * Returns the number of bytes consumed
 */
static int parse_modrm_sib(
    const uint8_t* code,
    int offset,
    x86_instruction_t* inst,
    uint8_t cflags,
    uint8_t* group_flags)
{
    uint8_t* p = (uint8_t*)(code + offset);
    int bytes_consumed = 0;

    /* Initialize to indicate no displacement */
    inst->disp_size = 0;

    /* Skip if no ModR/M byte */
    if (!(cflags & C_MODRM)) {
        return 0;
    }

    /* Parse ModR/M byte */
    uint8_t modrm = *p;
    inst->modrm = modrm;
    p++;
    bytes_consumed++;

    inst->flags |= F_MODRM;
    inst->modrm_mod = GET_MODRM_MOD(modrm);
    inst->modrm_reg = GET_MODRM_REG(modrm);
    inst->modrm_rm = GET_MODRM_RM(modrm);

    /* Get group opcode information if this is a group instruction */
    if (cflags & C_GROUP && group_flags != NULL) {
        *group_flags = x86_group_opcode_table[*group_flags - 1].entries[inst->modrm_reg].flags;
    }

    /* Parse SIB byte if needed */
    if (inst->modrm_mod != MODRM_MODE_REGISTER && inst->modrm_rm == 4) {
        uint8_t sib = *p;
        inst->flags |= F_SIB;
        inst->sib = sib;
        inst->sib_scale = GET_SIB_SCALE(sib);
        inst->sib_index = GET_SIB_INDEX(sib);
        inst->sib_base = GET_SIB_BASE(sib);

        p++;
        bytes_consumed++;

        /* Check for special case: no base with 32-bit displacement */
        if (inst->sib_base == 5 && inst->modrm_mod == MODRM_MODE_NO_DISP) {
            inst->flags |= F_DISP32;
            inst->disp_size = 4;
        }
    }

    /* Determine displacement size based on ModR/M */
    switch (inst->modrm_mod) {
    case MODRM_MODE_NO_DISP:
        /* No displacement unless special cases */
        if (inst->modrm_rm == 4 && inst->sib_base == 5) {
            /* SIB with no base + 32-bit disp */
            inst->disp_size = 4;
            inst->flags |= F_DISP32;
        }
        else if (inst->modrm_rm == 5) {
            /* RIP-relative or direct 32-bit displacement */
            if (inst->p_67) {
                inst->flags |= F_DISP32;
            }
            else {
                inst->flags |= (F_DISP32 | F_IP_RELATIVE);
            }
            inst->disp_size = 4;
        }
        break;

    case MODRM_MODE_DISP8:
        /* 8-bit displacement */
        inst->flags |= F_DISP8;
        inst->disp_size = 1;
        break;

    case MODRM_MODE_DISP32:
        /* 32-bit displacement */
        inst->flags |= F_DISP32;
        inst->disp_size = 4;
        break;

    case MODRM_MODE_REGISTER:
        /* Register mode, no displacement */
        break;
    }

    return bytes_consumed;
}

/*
 * Parse displacement bytes if present
 * Returns the number of bytes consumed
 */
static int parse_displacement(
    const uint8_t* code,
    int offset,
    x86_instruction_t* inst) 
{
    uint8_t* p = (uint8_t*)(code + offset);
    int bytes_consumed = 0;

    /* Skip if no displacement */
    if (inst->disp_size == 0) {
        return 0;
    }

    /* Read displacement bytes */
    switch (inst->disp_size) {
    case 1:
        inst->disp.disp8 = *p;
        bytes_consumed = 1;
        break;

    case 2:
        /* Read 16-bit value safely */
        inst->disp.disp16 = p[0] | (p[1] << 8);
        bytes_consumed = 2;
        break;

    case 4:
        /* Read 32-bit value safely */
        inst->disp.disp32 = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
        bytes_consumed = 4;
        break;
    }

    return bytes_consumed;
}

/*
 * Parse immediate value if present
 * Returns the number of bytes consumed
 */
static int parse_immediate(
    const uint8_t* code,
    int offset,
    x86_instruction_t* inst,
    imm_type_t imm_type) 
{
    uint8_t* p = (uint8_t*)(code + offset);
    int bytes_consumed = 0;

    /* Skip if no immediate */
    if (imm_type == IMM_TYPE_NONE) {
        return 0;
    }

    /* Set appropriate flags and read immediate value */
    switch (imm_type) {
    case IMM_TYPE_8:
    case IMM_TYPE_REL8:
        inst->flags |= F_IMM8;
        inst->imm.imm8 = *p;
        inst->imm_size = 1;
        bytes_consumed = 1;
        break;

    case IMM_TYPE_16:
        inst->flags |= F_IMM16;
        /* Read 16-bit value safely */
        inst->imm.imm16 = p[0] | (p[1] << 8);
        inst->imm_size = 2;
        bytes_consumed = 2;
        break;

    case IMM_TYPE_32:
    case IMM_TYPE_REL32:
        inst->flags |= F_IMM32;
        /* Read 32-bit value safely */
        inst->imm.imm32 = p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
        inst->imm_size = 4;
        bytes_consumed = 4;
        break;

    case IMM_TYPE_64:
        inst->flags |= F_IMM64;
        /* Read 64-bit value safely */
        inst->imm.imm64 =
            ((uint64_t)p[0]) |
            ((uint64_t)p[1] << 8) |
            ((uint64_t)p[2] << 16) |
            ((uint64_t)p[3] << 24) |
            ((uint64_t)p[4] << 32) |
            ((uint64_t)p[5] << 40) |
            ((uint64_t)p[6] << 48) |
            ((uint64_t)p[7] << 56);
        inst->imm_size = 8;
        bytes_consumed = 8;
        break;

    default:
        /* No immediate or unknown type */
        break;
    }

    /* Mark as relative if applicable */
    if (imm_type == IMM_TYPE_REL8 || imm_type == IMM_TYPE_REL32) {
        inst->flags |= F_RELATIVE;
    }

    return bytes_consumed;
}

/*
 * Disassemble a single x86-64 instruction
 * Returns the number of bytes consumed
 */
int x86_disassembler(
    const uint8_t* code,
    x86_instruction_t* inst)
{
    int offset = 0;
    opcode_info_t opcode_info;
    uint8_t group_flags = 0;

    /* Initialize instruction */
    memset(inst, 0, sizeof(x86_instruction_t));

    /* Stage 1: Parse prefixes */
    offset += parse_prefixes(code, offset, inst);

    /* Stage 2: Parse REX prefix */
    offset += parse_rex(code, offset, inst);

    /* Stage 3: Parse opcode */
    offset += parse_opcode(code, offset, inst, &opcode_info);

    /* Stage 4: Parse ModR/M and SIB */
    group_flags = opcode_info.group_index;
    offset += parse_modrm_sib(code, offset, inst, opcode_info.flags, &group_flags);

    /* Stage 5: Parse displacement */
    offset += parse_displacement(code, offset, inst);

    /* Stage 6: Parse immediate value */
    offset += parse_immediate(code, offset, inst, opcode_info.imm_type);

    /* Finalize instruction */
    inst->length = offset;
    memcpy(inst->bytes, code, offset);

    return offset;
}