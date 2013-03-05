/*-------------------------------------*\
|
|  File Name: Int.hpp
|
|  Creation Date: 04-03-2012
|
|  Last Modified: Mon, Mar 04, 2013  2:41:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Int : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			INT_3		= 0xCC,
			INT_IMM8	= 0xCD,
			INTO		= 0xCE,
		};

	protected:
		Int(Prefix* pre, std::string text, std::string inst, int op);
};
