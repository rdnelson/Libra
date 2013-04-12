/*-------------------------------------*\
|
|  File Name: Jmp.hpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Sun, Oct 14, 2012  5:52:37 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Jmp : public Instruction8086 {

	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			JMP_REL8	= 0xEB,
			JMP_REL16	= 0xE9,
			JMP_MOD16	= 0xFF,
			JMP_PTR16_16	= 0xEA,
			JMP_MOD16_16	= 0xFF			

		};

		static const unsigned int JMP_SUB_OPCODE = 0x04;
		static const unsigned int JMP_SUB_SEG_OPCODE = 0x05;

	private:
		Jmp(Prefix* pre, std::string text, std::string inst, int op);

		unsigned int mType;
};
