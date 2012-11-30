/*-------------------------------------*\
|
|  File Name: Cwd.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Fri, Oct 12, 2012 10:08:59 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Cwd : public Instruction {
	public: 
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			CWD = 0x99,
		};

	protected:
		Cwd(Prefix* pre, std::string text, std::string inst, int op);
};
