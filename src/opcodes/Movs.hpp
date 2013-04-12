/*-------------------------------------*\
|
|  File Name: Movs.hpp
|
|  Creation Date: 04-03-2012
|
|  Last Modified: Mon, Mar 04, 2013  2:41:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction8086.hpp"

class Movs : public Instruction8086 {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute();

		enum eValidOpcodes {
			MOVSB		= 0xA4,
			MOVSW		= 0xA5,
		};

	protected:
		Movs(Prefix* pre, std::string text, std::string inst, int op);
};
