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
			CMP_MOD8_IMM8		= 0x80,
			CMP_MOD16_IMM16		= 0x81,
			CMP_MOD16_IMM8		= 0x83,
			CMP_MOD8_REG8		= 0x38,
			CMP_MOD16_REG16		= 0x39,

		};

		enum eValidGroup {
			GRP1_ADD_MOD_IMM8	= 0x80,
			GRP1_ADD_MOD_IMM16	= 0x81,
			GRP1_ADD_MOD_SIMM8	= 0x83,
		};


	private:
		Cmp(Prefix* pre, std::string text, std::string inst, int op);

};

