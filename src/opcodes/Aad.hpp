/*-------------------------------------*\
|
|  File Name: Aad.hpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  1:58:35 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Aad : public Instruction8086 {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			AAD = 0xD5
		};

	protected:
		Aad(Prefix* pre, std::string text, std::string inst, int op);
};
