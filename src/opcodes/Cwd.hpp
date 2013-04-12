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

#include "../Instruction8086.hpp"

class Cwd : public Instruction8086 {
	public: 
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			CWD = 0x99,
		};

	protected:
		Cwd(Prefix* pre, std::string text, std::string inst, int op);
};
