/*-------------------------------------*\
|
|  File Name: Not.hpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thurs, Oct 18, 2012  7:19:00 PM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Not : public Instruction {

	public:
		static Instruction* CreateInstruction(Memory& memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			NOT_MOD8	= 0xF6,
			NOT_MOD16	= 0xF7,
		};

	protected:
		Not(Prefix* pre, std::string text, std::string inst, int op);
};
