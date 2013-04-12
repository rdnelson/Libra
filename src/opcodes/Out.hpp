/*-------------------------------------*\
|
|  File Name: Out.hpp
|
|  Creation Date: 20-10-2012
|
|  Last Modified: Sat, Oct 20, 2012  2:41:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Out : public Instruction8086 {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			OUT_IMM8_AL	= 0xE6,
			OUT_IMM8_AX	= 0xE7,
			OUT_DX_AL	= 0xEE,
			OUT_DX_AX	= 0xEF,
		};

	protected:
		Out(Prefix* pre, std::string text, std::string inst, int op);
};
