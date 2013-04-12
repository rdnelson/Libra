/*-------------------------------------*\
|
|  File Name: Or.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tues, Oct  9, 2012  9:54:26 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Processor8086;

class Or: public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			OR_AL_IMM8		= 0x0C,
			OR_AX_IMM16		= 0x0D,
		};

		enum eValidGroup {
			GRP1_OR_MOD8_IMM8		= 0x80,
			GRP1_OR_MOD16_IMM8		= 0x83,
			GRP1_OR_MOD16_IMM16		= 0x81,

			GRP2_OR_MOD8_REG8		= 0x08,
			GRP2_OR_MOD16_REG16		= 0x09,

			GRP3_OR_REG8_MOD8		= 0x0A,
			GRP3_OR_REG16_MOD16		= 0x0B,
		};


	private:
		Or(Prefix* pre, std::string text, std::string inst, int op);

};

