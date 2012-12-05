/*-------------------------------------*\
|
|  File Name: Sub.hpp
|
|  Creation Date: 17-10-2012
|
|  Last Modified: Wed, Oct  17, 2012  9:54:26 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#pragma once

#include "Cmp.hpp"
#include "../Instruction.hpp"

class Processor;

class Sub: public Instruction {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			SUB_AL_IMM8			= 0x2C,
			SUB_AX_IMM16		= 0x2D,
		};

		enum eValidGroup {
			GRP1_SUB_MOD8_IMM8		= 0x80,
			GRP1_SUB_MOD16_IMM8		= 0x83,
			GRP1_SUB_MOD16_IMM16	= 0x81,

			GRP2_SUB_MOD8_REG8		= 0x28,
			GRP2_SUB_MOD16_REG16	= 0x29,

			GRP3_SUB_REG8_MOD8		= 0x2A,
			GRP3_SUB_REG16_MOD16	= 0x2B,
		};


	private:
		Sub(Prefix* pre, std::string text, std::string inst, int op);

};

