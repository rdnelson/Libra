/*-------------------------------------*\
|
|  File Name: Cmp.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tues, Oct  9, 2012  9:54:26 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Processor;

class Cmp: public Instruction {

	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			CMP_AL_IMM8			= 0x3C,
			CMP_AX_IMM16		= 0x3D,
		};

		enum eValidGroup {
			GRP1_CMP_MOD8_IMM8		= 0x80,
			GRP1_CMP_MOD16_IMM16	= 0x81,
			GRP1_CMP_MOD16_IMM8		= 0x83,
			GRP1_CMP_MOD8_REG8		= 0x38,
			GRP1_CMP_MOD16_REG16	= 0x39,
			GRP1_CMP_REG8_MOD8		= 0x3A,
			GRP1_CMP_REG16_MOD16	= 0x3B,
		};


	private:
		Cmp(Prefix* pre, std::string text, std::string inst, int op);

};

