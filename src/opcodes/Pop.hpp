/*-------------------------------------*\
|
|  File Name: Pop.hpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  7:58:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Pop : public Instruction {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			POP_MOD16	= 0x8F,
			POP_REG_AX	= 0x58,
			POP_REG_CX	= 0x59,
			POP_REG_DX	= 0x5A,
			POP_REG_BX	= 0x5B,
			POP_REG_SP	= 0x5C,
			POP_REG_BP	= 0x5D,
			POP_REG_SI	= 0x5E,
			POP_REG_DI	= 0x5F,
			POP_SS		= 0x17,
			POP_DS		= 0x1F,
			POP_ES		= 0x07,
		};

		static const unsigned int POP_SUB_OPCODE = 0x00;

	private:
		Pop(Prefix* pre, std::string text, std::string inst, int op);

};
