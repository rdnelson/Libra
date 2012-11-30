/*-------------------------------------*\
|
|  File Name: CmpsX.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  6:28:57 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class CmpsX : public Instruction {
	public: 
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			CMPSB = 0xA6,
			CMPSW = 0xA7,
		};

	protected:
		CmpsX(Prefix* pre, std::string text, std::string inst, int op);
};
