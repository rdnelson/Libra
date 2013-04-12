/*-------------------------------------*\
|
|  File Name: Aaa.hpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Wed, Oct 10, 2012 12:41:46 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Aaa : public Instruction8086 {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			AAA = 0x37
		};

	protected:
		Aaa(Prefix* pre, std::string text, std::string inst, int op);
};
