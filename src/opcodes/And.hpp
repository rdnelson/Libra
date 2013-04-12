/*-------------------------------------*\
|
|  File Name: And.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  3:29:12 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class And : public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			AND_AL_IMM8		= 0x24,
			AND_AX_IMM16		= 0x25,
			AND_MOD8_IMM8		= 0x80,
			AND_MOD16_IMM16		= 0x81,
			AND_MOD16_IMM8		= 0x83,
			AND_MOD8_REG8		= 0x20,
			AND_MOD16_REG16		= 0x21,
			AND_REG8_MOD8		= 0x22,
			AND_REG16_MOD16		= 0x23,
		};

		static const unsigned int AND_SUB_OPCODE = 0x04;

	protected:
		And(Prefix* pre, std::string text, std::string inst, int op);

};
