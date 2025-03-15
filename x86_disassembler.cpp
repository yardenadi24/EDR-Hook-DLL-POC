/*
 * x86-64 Disassembler
 * Complete implementation based on the HDE64 approach
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
 * Disassemble a single x86-64 instruction
 * Returns the number of bytes consumed
 */
int x86_disassembler(const uint8_t* code, x86_instruction_t* inst)
{
	uint8_t x = 0;
	uint8_t c = 0;
	uint8_t* p = (uint8_t*)code; // Byte pointer

	uint8_t cflags = C_NONE;

	uint8_t opcode = 0;
	uint8_t opcode2 = 0;
	uint8_t opcode3 = 0;

	uint8_t modrm_mod = 0;
	uint8_t modrm_reg = 0;
	uint8_t modrm_rm = 0;

	uint8_t sib = 0;
	uint8_t sib_base = 0;
	uint8_t sib_index = 0;
	uint8_t sib_scale = 0;

	uint8_t rex = 0;
	uint8_t rex_w = 0;
	uint8_t rex_b = 0;
	uint8_t rex_x = 0;

	uint8_t pref = 0;
	uint8_t valid_prefixes = 0;

	uint8_t disp_size = 0;

	int pos = 0;
	int op64 = 0;

	/* Init the instruction struct */
	memset(inst, 0, sizeof(x86_instruction_t));

	/*
	* Stage 1: Parse prefixes
	*/
	for (x = 0; x < MAX_LEGACY_PREFIXES; x++)
	{

		/* Set c to the current byte */
		c = *p;

		switch (c)
		{
		case PREFIX_REP:
			inst->p_rep = c;
			pref |= PRE_REP;
			inst->flags |= F_PREFIX_REP;
			inst->prefixes[inst->prefix_count] = c;
			inst->prefix_count++;
			p++;
			pos++;
			break;
		case PREFIX_REPNE:
			inst->p_repne = c;
			pref |= PRE_REPN;
			inst->flags |= F_PREFIX_REPNZ;
			inst->prefixes[inst->prefix_count] = c;
			inst->prefix_count++;
			p++;
			pos++;
			break;
		case PREFIX_LOCK:
			inst->p_lock = c;
			pref |= PRE_LOCK;
			inst->flags |= F_PREFIX_LOCK;
			inst->prefixes[inst->prefix_count] = c;
			inst->prefix_count++;
			p++;
			pos++;
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
			pos++;
			break;
		case PREFIX_OPERAND_SIZE:
			inst->p_66 = 1;
			pref |= PRE_OPERAND_OVERRIDE;
			inst->flags |= F_PREFIX_66;
			inst->prefixes[inst->prefix_count] = c;
			inst->prefix_count++;
			p++;
			pos++;
			break;
		case PREFIX_ADDRESS_SIZE:
			inst->p_67 = 1;
			pref |= PRE_ADDRESS_OVERRIDE;
			inst->flags |= F_PREFIX_67;
			inst->prefixes[inst->prefix_count] = c;
			inst->prefix_count++;
			p++;
			pos++;
			break;
		default:
			goto legacy_prefix_done;
		}
	}


legacy_prefix_done:

	/*
	* Stage 2: Parse REX prefix in the instructions
	*/
	c = *p;
	if (IS_REX_BYTE(c))
	{
		inst->flags |= F_PREFIX_REX;
		inst->rex = c;

		/* Parse the REX byte */
		inst->rex_w = GET_REX_W(c);
		inst->rex_r = GET_REX_R(c);
		inst->rex_x = GET_REX_X(c);
		inst->rex_b = GET_REX_B(c);

		p++;
		pos++;
	}

	/*
	* Stage 3: Parse opcode
	*/

	opcode = *p;
	p++;
	pos++;
	inst->opcode = opcode;

	/* Check for MOV r64, imm64 - Special case instruction (REX.w and ) */
	if (inst->rex_w && opcode >= 0xB8 && opcode <= 0xBF) {
		op64 = 1;
	}

	/* Check for 2/3 byte opcode */
	if (opcode == TWO_BYTE_OPCODE)
	{
		/* 2byte opcode */
		opcode2 = *p;
		p++;
		pos++;
		inst->opcode2 = opcode2;

		/* Check is 3byte opcode */
		if (opcode2 == THREE_BYTE_OPCODE_38 || opcode2 == THREE_BYTE_OPCODE_3A)
		{
			opcode3 = *p;
			p++;
			pos++;
			inst->opcode3 = opcode3;
		}
	}

	/*
	* Stage 4: Lookup opcode characteristic from tables
	*/
	if (opcode2)
	{
		/* 2/3 byte opcode */
		if (opcode3)
		{
			/* 3 byte opcode */
			if (opcode2 == THREE_BYTE_OPCODE_38)
			{
				cflags = x86_3byte_38_opcode_table[opcode3].flags;
				valid_prefixes = x86_3byte_38_opcode_table[opcode3].prefix;

			}
			else if (opcode2 == THREE_BYTE_OPCODE_3A)
			{
				cflags = x86_3byte_3A_opcode_table[opcode3].flags;
				valid_prefixes = x86_3byte_3A_opcode_table[opcode3].prefix;
			}
		}
		else {
			/* 2 byte opcode */
			cflags = x86_2byte_opcode_table[opcode2].flags;
			valid_prefixes = x86_2byte_opcode_table[opcode2].prefix;
		}
	}
	else {
		/* 1 byte opcode */
		cflags = x86_1byte_opcode_table[opcode].flags;
		valid_prefixes = x86_1byte_opcode_table[opcode].prefix;

		/*	Some special cases for certain opcodes
			MOV instructions 0xA0 - 0XA3 using moffs (memory offset)
			0xA0 - MOV	AL, 			moffs8
			0xA1 - MOV	RAX,			moffs16/32/64
			0xA2 - MOV	moffs8,			AL
			0xA3 - MOV	moffs16/32/64,	rAX

			Where moffs is an absolute memory address of the size 8/16/32/64 bit
			In this case 0x66 (operand size) and 0x67 (Address size) operate different.

							INST:	0xA0: MOV AL, moffs8 (Load byte to AL)
							None:	64bit address, 8bit load (1byte)
			(Operand size)	0x66:	No effect operand is fixed -> Still uses 8bit operand
			(Address size)	0x67:	32bit address, 8bit load
							Both:	32bit address, 8bit load

							INST:	0xA1: MOV RAX, moffs16/32/64 (Load to AX/EAX/RAX)
							None:	64bit address, 64bit load (8bytes)
			(Operand size)	0x66:	64bit address, 16bit load (2bytes)
			(Address size)	0x67:	32bit address, 32bit load (Address is 32bit which causes the reg be 32bit instead of 64bit)
							Both:	32bit address, 16bit load

							INST:	0xA2: MOV moffs8, AL (Store byte from AL)
							None:	64bit address, 8bit store
			(Operand size)	0x66:	No effect operand is fixed -> Still uses 8bit operand
			(Address size)	0x67:	32bit address, 8bit store
							Both:	32bit address, 8bit store

							INST:	0xA3: MOV moffs16/32/64, RAX (Store from AX/EAX/RAX)
							None:	64bit address, 64bit store
			(Operand size)	0x66:	64bit address, 16bit store
			(Address size)	0x67:	32bit address, 32bit store
							Both:	32bit address, 32bit store
		*/
		if (opcode >= 0xA0 && opcode <= 0xA3)
		{
			op64++;
			if (inst->flags & F_PREFIX_67)
			{
				// Address size prefix affects the operand itself
				inst->flags |= F_PREFIX_66;
			}
			//else {
			//	inst->flags &= ~F_PREFIX_66;
			//}
		}
	}

	/*
	* Stage 5: Parse ModR/M and groups
	*/

	if (cflags & C_MODRM)
	{
		c = *p;
		inst->modrm = c;
		p++;
		pos++;

		inst->flags |= F_MODRM;
		modrm_mod = inst->modrm_mod = GET_MODRM_MOD(c);
		modrm_reg = inst->modrm_reg = GET_MODRM_REG(c);
		modrm_rm = inst->modrm_rm = GET_MODRM_RM(c);

	}

	if (cflags & C_GROUP)
	{
		uint8_t group_index = 0;

		// Group index is stored in the 'extra' field of the opcode entry
		if (opcode2) {
			group_index = x86_2byte_opcode_table[opcode2].group;
		}
		else {
			group_index = x86_1byte_opcode_table[opcode].group;
		}

		/* Get the opcode characteristics based on ModR/M.reg field */
		cflags |= x86_group_opcode_table[group_index - 1].entries[modrm_reg].flags;
		valid_prefixes |= x86_group_opcode_table[group_index - 1].entries[modrm_reg].prefix;

	}

	/*
	* Stage 6: Parse SIB byte
	*/
	if (inst->flags & F_MODRM)
	{
		/*	MODRM.mod - 11 is Register mode so no SIB
		*   Rest of MODRM.mod, if MODRM.rm is 100 then we use SIB
		*/
		if (modrm_mod != 3 && modrm_rm == 4)
		{
			inst->flags |= F_SIB;
			inst->sib = sib = *p;
			inst->sib_scale = GET_SIB_SCALE(sib);
			inst->sib_index = GET_SIB_INDEX(sib);
			inst->sib_base = GET_SIB_BASE(sib);

			sib_scale = inst->sib_scale;
			sib_index = inst->sib_index;
			sib_base = inst->sib_base;

			p++;
			pos++;

			/*
			 * Check for special addressing modes
			 * Modrm.mod = 00 which indicates no displacement
			 * With SIB.base = 101 which means 32-bit displacement
			 */
			if (sib_base == 5 && modrm_mod == 0)
			{
				inst->flags |= F_DISP32;
				disp_size = 4; /*4 Bytes*/
			}
		}

		/*
		* Stage 7: Parse displacement
		*/
		switch (inst->modrm_mod)
		{
		case 0: /* Memory mode, no displacement */
			if (modrm_rm == 4 && sib_base == 5) /* SIB follows AND Special: No base, 32-bit displacement */
			{
				disp_size = 4;
				inst->flags |= F_DISP32;
			}
			else if (modrm_rm == 5)
			{
				if (inst->p_67)
				{
					inst->flags |= F_DISP32;
				}
				else
				{
					inst->flags |= (F_DISP32 | F_IP_RELATIVE);
				}

				disp_size = 4;
			}
			break;
		case 1: /* Memory mode with 8-bit displacement */
			inst->flags |= F_DISP8;
			disp_size = 1;
			break;
		case 2: /* Memory mode with 32-bit displacement */
			inst->flags |= F_DISP32;
			disp_size = 4;
			break;
		}

		/* Read displacement bytes */
		if (disp_size > 0)
		{
			switch (disp_size)
			{
			case 1:
				inst->flags |= F_DISP8;
				inst->disp.disp8 = *p;
				break;
			case 2:
				inst->flags |= F_DISP16;
				inst->disp.disp16 = *(uint16_t*)p;
				break;
			case 4:
				inst->flags |= F_DISP32;
				inst->disp.disp32 = *(uint32_t*)p;
				break;
			}

			inst->disp_size = disp_size;
			p += disp_size;
			pos += disp_size;
		}

	}

	/*
	* Stage 8: Parse immediate value if needed
	*/
	if (cflags & (C_IMM_P66 | C_IMM16 | C_IMM8 | C_REL32 | C_REL8))
	{
		int imm_size = 0;         // Size of immediate in bytes
		uint32_t imm_flag = 0;    // Flag for immediate type
		bool is_relative = false; // Whether this is a relative address

		// Special case for A0-A3 (direct memory MOV instructions)
		if (opcode >= 0xA0 && opcode <= 0xA3) {
			// Handle the immediate (memory address)
			if (inst->p_67) {
				// 32-bit address mode
				inst->flags |= F_IMM32;
				imm_size = 4;
				imm_flag = F_IMM32;
			}
			else {
				// 64-bit address mode
				inst->flags |= F_IMM64;
				imm_size = 8;
				imm_flag = F_IMM64;
			}
		}
		else if (cflags & C_IMM_P66)
		{

			if (cflags & C_REL32)
			{
				// Relative address with size depending on prefix
				is_relative = true;
				if (inst->flags & F_PREFIX_66)
				{
					inst->flags |= F_IMM16;
					imm_size = 2;
					imm_flag = F_IMM16;
				}
				else {
					inst->flags |= F_IMM32;
					imm_size = 4;
					imm_flag = F_IMM32;
				}
			}
			else if (op64)
			{
				inst->flags |= F_IMM64;
				imm_size = 8;
				imm_flag = F_IMM64;
			}
			else if (inst->flags & F_PREFIX_66)
			{
				inst->flags |= F_IMM16;
				imm_size = 2;
				imm_flag = F_IMM16;
			}
			else {
				inst->flags |= F_IMM32;
				imm_size = 4;
				imm_flag = F_IMM32;
			}

		}
		else if (cflags & C_IMM16)
		{
			inst->flags |= F_IMM16;
			imm_size = 2;
			imm_flag = F_IMM16;
		}
		else if (cflags & C_IMM8)
		{
			inst->flags |= F_IMM8;
			imm_size = 1;
			imm_flag = F_IMM8;
		}
		else if (cflags & C_REL32)
		{
			inst->flags |= F_IMM32;
			imm_size = 4;
			imm_flag = F_IMM32;
			is_relative = true;
		}
		else if (cflags & C_REL8)
		{
			inst->flags |= F_IMM8;
			imm_size = 1;
			imm_flag = F_IMM8;
			is_relative = true;
		}


		inst->flags |= imm_flag;
		if (is_relative)
		{
			inst->flags |= F_RELATIVE;
		}

		switch (imm_size)
		{
		case 1:
			inst->imm.imm8 = *p;
			break;
		case 2:
			inst->imm.imm16 = *(uint16_t*)p;
			break;
		case 4:
			inst->imm.imm32 = *(uint32_t*)p;
			break;
		case 8:
			inst->imm.imm64 = *(uint64_t*)p;
			break;
		}

		inst->imm_size = imm_size;
		p += imm_size;
		pos += imm_size;


	}

	inst->length = pos;
	memcpy(inst->bytes, code, pos);
	return pos;
}
