/*-------------------------------------*\
|
|  File Name: Mov.hpp
|
|  Creation Date: 03-10-2012
|
|  Last Modified: Thu, Oct  4, 2012  6:29:19 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once 

#include "../Instruction.hpp"
#include "../Prefix.hpp"
#include <string>


class Mov : public Instruction {

	public:
		Mov(Prefix* proc, std::string, std::string, int);
		static Instruction* CreateInstruction(Memory& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			MOV_MOD8_REG8		= 0x88,
			MOV_MOD16_REG16		= 0x89,
			MOV_REG8_MOD8		= 0x8A,
			MOV_REG16_MOD16		= 0x8B,
			MOV_MOD16_SEGREG	= 0x8C,
			MOV_SEGREG_MOD16	= 0x8E,
			MOV_AL_MOFF8		= 0xA0,
			MOV_AX_MOFF16		= 0xA1,
			MOV_MOFF8_AL		= 0xA2,
			MOV_MOFF16_AX		= 0xA3,
			MOV_AL_IMM8		= 0xB0,
			MOV_CL_IMM8		= 0xB1,
			MOV_DL_IMM8		= 0xB2,
			MOV_BL_IMM8		= 0xB3,
			MOV_AH_IMM8		= 0xB4,
			MOV_CH_IMM8		= 0xB5,
			MOV_DH_IMM8		= 0xB6,
			MOV_BH_IMM8		= 0xB7,
			MOV_AX_IMM16		= 0xB8,
			MOV_CX_IMM16		= 0xB9,
			MOV_DX_IMM16		= 0xBA,
			MOV_BX_IMM16		= 0xBB,
			MOV_SP_IMM16		= 0xBC,
			MOV_BP_IMM16		= 0xBD,
			MOV_SI_IMM16		= 0xBE,
			MOV_DI_IMM16		= 0xBF,
			MOV_MOD8_IMM8		= 0xC6,
			MOV_MOD16_IMM16		= 0xC7,
		};


};
