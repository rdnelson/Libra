/*-------------------------------------*\
|
|  File Name: Daa.hpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Wed, Oct 10, 2012 12:41:46 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Daa : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			DAA = 0x27
		};

	protected:
		Daa(Prefix* pre, std::string text, std::string inst, int op);
};
