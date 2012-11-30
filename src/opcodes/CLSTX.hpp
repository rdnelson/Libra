/*-------------------------------------*\
|
|  File Name: CLSTX.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  1:34:36 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class CLSTX : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			CLC	= 0xF8,
			CLD	= 0xFC,
			CLI	= 0xFA,
			CMC	= 0xF5,
			STC	= 0xF9,
			STD	= 0xFD,
			STI	= 0xFB,
		};

	protected:
		CLSTX(Prefix* pre, std::string text, std::string inst, int op);
};
