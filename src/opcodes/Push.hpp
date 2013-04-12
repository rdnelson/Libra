/*-------------------------------------*\
|
|  File Name: Push.hpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  7:58:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Push : public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			PUSH_MOD16	= 0xFF,
			PUSH_REG_AX	= 0x50,
			PUSH_REG_CX	= 0x51,
			PUSH_REG_DX	= 0x52,
			PUSH_REG_BX	= 0x53,
			PUSH_REG_SP	= 0x54,
			PUSH_REG_BP	= 0x55,
			PUSH_REG_SI	= 0x56,
			PUSH_REG_DI	= 0x57,
			PUSH_IMM8	= 0x6A,
			PUSH_IMM16	= 0x68,
			PUSH_CS		= 0x0E,
			PUSH_SS		= 0x16,
			PUSH_DS		= 0x1E,
			PUSH_ES		= 0x06,
			PUSHF		= 0x9C,
			PUSHA		= 0x60
		};

		static const unsigned int PUSH_SUB_OPCODE = 0x06;

	private:
		Push(Prefix* pre, std::string text, std::string inst, int op);

};
