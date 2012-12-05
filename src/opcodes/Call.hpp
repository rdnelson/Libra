/*-------------------------------------*\
|
|  File Name: Call.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Sun, Oct 14, 2012  5:33:59 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Call : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			CALL_REL16	= 0xE8,
			CALL_MOD16	= 0xFF,
			CALL_ABS16_16	= 0x9A,
			CALL_IND16_16	= 0xFF,
		};

		static const unsigned int CALL_MOD_CONST = 0x02;
		static const unsigned int CALL_IND_CONST = 0x03;

	protected:
		Call(Prefix* pre, std::string text, std::string inst, int op);
		unsigned int mType;

};
