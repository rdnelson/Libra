/*-------------------------------------*\
|
|  File Name: StiCli.hpp
|
|  Creation Date: 04-03-2012
|
|  Last Modified: Mon, Mar 04, 2013  2:41:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class StiCli : public Instruction {
	public:
		static Instruction* CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			STI		= 0xFB,
			CLI		= 0xFA,
		};

	protected:
		StiCli(Prefix* pre, std::string text, std::string inst, int op);
};
