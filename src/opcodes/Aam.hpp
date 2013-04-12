/*-------------------------------------*\
|
|  File Name: Aam.hpp
|
|  Creation Date: 09-10-2012
|
|  Last Modified: Tue, Oct  9, 2012  9:12:38 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Aam : public Instruction8086 {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			AAM = 0xD4
		};

	protected:
		Aam(Prefix* pre, std::string text, std::string inst, int op);
};
