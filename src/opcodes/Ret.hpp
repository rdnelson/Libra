/*-------------------------------------*\
|
|  File Name: Ret.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tue, Oct  9, 2012  3:52:42 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Ret : public Instruction8086 {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			RET_NEAR	= 0xC3,
			RET_FAR		= 0xCB,
			RET_NEAR_POP	= 0xC2,
			RET_FAR_POP	= 0xCA
		};

	protected:
		Ret(Prefix* pre, std::string text, std::string inst, int op);
};
