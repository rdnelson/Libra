/*-------------------------------------*\
|
|  File Name: In.hpp
|
|  Creation Date: 20-10-2012
|
|  Last Modified: Sat, Oct 20, 2012  2:41:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class In : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			IN_AL_IMM8	= 0xE4,
			IN_AX_IMM8	= 0xE5,
			IN_AL_DX	= 0xEC,
			IN_AX_DX	= 0xED,
		};

	protected:
		In(Prefix* pre, std::string text, std::string inst, int op);
};
