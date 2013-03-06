/*-------------------------------------*\
|
|  File Name: Sbb.hpp
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

class Sbb: public Instruction {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			SBB_AL_IMM8		= 0x1C,
			SBB_AX_IMM16		= 0x1D,
			SBB_MOD8_REG8		= 0x18,
			SBB_MOD16_REG16		= 0x19,

			SBB_REG8_MOD8		= 0x1A,
			SBB_REG16_MOD16		= 0x1B,

		};

		enum eValidGroup {
			GRP1_SBB_MOD8_IMM8	= 0x80,
			GRP1_SBB_MOD16_IMM8	= 0x83,
			GRP1_SBB_MOD16_IMM16	= 0x81,
		};


	private:
		Sbb(Prefix* pre, std::string text, std::string inst, int op);

};

