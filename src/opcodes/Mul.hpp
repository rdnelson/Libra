/*-------------------------------------*\
|
|  File Name: Mul.hpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  9:19:00 AM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#pragma once

#include "../Instruction.hpp"

class Mul : public Instruction {

	public:
		static Instruction* CreateInstruction(unsigned char* memLoc, Processor* proc);
		int Execute(Processor* proc);

		enum eValidOpcodes {
			MUL_MOD8	= 0xF6,
			MUL_MOD16	= 0xF7,
		};

	protected:
		Mul(Prefix* pre, std::string text, std::string inst, int op);
};
